#!/usr/bin/env python3
"""
Enhanced Docker-friendly benchmark script for matrix multiplication.
Supports 3 test cases: small values, large values, and random values.
"""

import argparse
import subprocess
import sys
import time
import csv
import os
from typing import List, Tuple
import matplotlib.pyplot as plt
import numpy as np


class DockerMatMulBenchmark:
    """Enhanced benchmark class for Docker environment."""

    def __init__(self, executable_path: str, cutoff: int = 64):
        """
        Initialize the benchmark.

        Args:
            executable_path: Path to C++ executable with multiplication functions
            cutoff: Cutoff value for Strassen algorithm
        """
        self.executable_path = executable_path
        self.cutoff = cutoff

    def generate_test_code(self, algorithm: str, size: int, min_val: int = -10, max_val: int = 10, seed: int = 42) -> str:
        """
        Generate C++ code to test a specific algorithm.

        Args:
            algorithm: "naive" or "strassen"
            size: Size of square matrix
            min_val: Minimum value for matrix elements
            max_val: Maximum value for matrix elements
            seed: Seed for random number generation

        Returns:
            C++ code as string
        """
        if algorithm == "naive":
            function_call = f"matmul_naive(A, B)"
        elif algorithm == "strassen":
            function_call = f"matmul_strassen(A, B, {self.cutoff})"
        else:
            raise ValueError(f"Unknown algorithm: {algorithm}")

        return f"""
#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include "./algorithms/cpp/luAlgorithms.h"

using namespace std;

static Matrix generate_random_square_matrix(int n, int min_val, int max_val, unsigned seed) {{
    Matrix M(n);
    mt19937 rng(seed);
    uniform_int_distribution<int> dist(min_val, max_val);
    for (int i = 0; i < n; i++) {{
        for (int j = 0; j < n; j++) {{
            M[i][j] = dist(rng);
        }}
    }}
    return M;
}}

int main() {{
    const int n = {size};
    cout << "Testing {algorithm} with n=" << n << ", values=[{min_val}, {max_val}]" << endl;

    // Generate test matrices
    Matrix A = generate_random_square_matrix(n, {min_val}, {max_val}, {seed});
    Matrix B = generate_random_square_matrix(n, {min_val}, {max_val}, {seed + 1});

    // Mark start time
    auto start = chrono::high_resolution_clock::now();

    // Execute multiplication
    Matrix C = {function_call};

    // Mark end time
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;

    cout << "Time: " << duration.count() << " seconds" << endl;
    cout << "Result size: " << C.size() << "x" << C[0].size() << endl;

    return 0;
}}
"""

    def run_single_test(self, algorithm: str, size: int, min_val: int = -10, max_val: int = 10, seed: int = 42) -> float:
        """
        Execute a single benchmark test.

        Args:
            algorithm: Algorithm to test ("naive" or "strassen")
            size: Matrix size
            min_val: Minimum value for matrix elements
            max_val: Maximum value for matrix elements
            seed: Seed for reproducibility

        Returns:
            Execution time in seconds
        """
        # Generate test code
        test_code = self.generate_test_code(algorithm, size, min_val, max_val, seed)

        # Create temporary file
        temp_filename = f"temp_benchmark_{algorithm}_{size}_{min_val}_{max_val}.cpp"
        with open(temp_filename, 'w') as f:
            f.write(test_code)

        try:
            # Compile code with optimization
            compile_cmd = f"g++ -std=c++17 -O3 -I. {temp_filename} ./algorithms/cpp/luAlgorithms.cpp -o temp_benchmark_{algorithm}_{size}_{min_val}_{max_val}"
            result = subprocess.run(compile_cmd, shell=True, capture_output=True, text=True)

            if result.returncode != 0:
                print(f"Compilation error for {algorithm} n={size} values=[{min_val}, {max_val}]:")
                print(result.stderr)
                return float('inf')

            # Execute benchmark
            run_cmd = f"./temp_benchmark_{algorithm}_{size}_{min_val}_{max_val}"
            result = subprocess.run(run_cmd, shell=True, capture_output=True, text=True)

            if result.returncode != 0:
                print(f"Execution error for {algorithm} n={size} values=[{min_val}, {max_val}]:")
                print(result.stderr)
                return float('inf')

            # Extract time from output
            output = result.stdout
            for line in output.split('\n'):
                if line.startswith('Time:'):
                    time_str = line.split(':')[1].strip().split()[0]
                    return float(time_str)

            print(f"Could not extract time from output for {algorithm} n={size}")
            return float('inf')

        except Exception as e:
            print(f"Error executing test {algorithm} n={size}: {e}")
            return float('inf')

        finally:
            # Clean up temporary files
            for ext in ['.cpp', '']:
                temp_file = f"temp_benchmark_{algorithm}_{size}_{min_val}_{max_val}{ext}"
                if os.path.exists(temp_file):
                    os.remove(temp_file)

    def run_benchmark(self, sizes: List[int], num_runs: int = 1, min_val: int = -10, max_val: int = 10) -> List[Tuple[int, float, float]]:
        """
        Execute complete benchmarks for all sizes.

        Args:
            sizes: List of sizes to test
            num_runs: Number of executions per test for average
            min_val: Minimum value for matrix elements
            max_val: Maximum value for matrix elements

        Returns:
            List of tuples (size, naive_time, strassen_time)
        """
        results = []

        print(f"Starting benchmark with {len(sizes)} sizes and {num_runs} executions per test")
        print(f"Matrix values: [{min_val}, {max_val}]")
        print("=" * 60)

        for size in sizes:
            print(f"\nTesting n={size}")

            naive_times = []
            strassen_times = []

            # Execute multiple times for each algorithm
            for run in range(num_runs):
                print(f"  Run {run + 1}/{num_runs}...")

                # Test naive
                naive_time = self.run_single_test("naive", size, min_val, max_val, seed=42 + run)
                if naive_time != float('inf'):
                    naive_times.append(naive_time)

                # Test strassen
                strassen_time = self.run_single_test("strassen", size, min_val, max_val, seed=42 + run)
                if strassen_time != float('inf'):
                    strassen_times.append(strassen_time)

            # Calculate averages
            avg_naive = sum(naive_times) / len(naive_times) if naive_times else float('inf')
            avg_strassen = sum(strassen_times) / len(strassen_times) if strassen_times else float('inf')

            results.append((size, avg_naive, avg_strassen))
            print(f"  Naive: {avg_naive:.6f}s")
            print(f"  Strassen: {avg_strassen:.6f}s")

        return results

    def save_results(self, results: List[Tuple[int, float, float]], filename: str):
        """
        Save results to CSV file.

        Args:
            results: List of results (size, naive_time, strassen_time)
            filename: CSV filename
        """
        # Ensure results directory exists
        os.makedirs(os.path.dirname(filename), exist_ok=True)
        
        with open(filename, 'w', newline='') as csvfile:
            writer = csv.writer(csvfile)
            writer.writerow(['size', 'naive_time', 'strassen_time'])
            for size, naive_time, strassen_time in results:
                writer.writerow([size, naive_time, strassen_time])

        print(f"Results saved to {filename}")

    def plot_results(self, results: List[Tuple[int, float, float]], filename: str = None, theory_curves: List[str] = None):
        """
        Generate comparative plot of results.

        Args:
            results: List of results (size, naive_time, strassen_time)
            filename: PNG filename to save plot (optional)
            theory_curves: List of theoretical curves to display
        """
        sizes = [r[0] for r in results]
        naive_times = [r[1] for r in results]
        strassen_times = [r[2] for r in results]

        plt.figure(figsize=(12, 8))

        # Plot experimental times
        plt.plot(sizes, naive_times, 'o-', label='Naive', linewidth=2, markersize=6)
        plt.plot(sizes, strassen_times, 's-', label='Strassen (cutoff=64)', linewidth=2, markersize=6)

        # Add theoretical complexity curves
        if sizes:
            min_size = min(sizes)
            min_idx = sizes.index(min_size)

            naive_min_n = naive_times[min_idx]
            strassen_min_n = strassen_times[min_idx]

            base_candidates = [t for t in [naive_min_n, strassen_min_n] if t != float('inf')]
            if len(base_candidates) == 0:
                base_candidates = [t for t in (naive_times + strassen_times) if t != float('inf')]
            if len(base_candidates) == 0:
                base_time = 1.0
            else:
                base_time = sum(base_candidates) / len(base_candidates)

            theory_sizes = np.linspace(min_size, max(sizes), 100)

            o_n3 = base_time * (theory_sizes / min_size) ** 3.0
            o_n281 = base_time * (theory_sizes / min_size) ** 2.807
            o_n2 = base_time * (theory_sizes / min_size) ** 2.0

            curves = set(c.lower() for c in (theory_curves or ["n3", "n2.81", "n2"]))
            if any(tag in curves for tag in ["n3", "n^3", "o(n^3)", "o(n3)"]):
                plt.plot(theory_sizes, o_n3, '--', color='blue', alpha=0.7, label='O(n³)', linewidth=1.5)
            if any(tag in curves for tag in ["n2.81", "n^2.81", "o(n^2.81)"]):
                plt.plot(theory_sizes, o_n281, '--', color='red', alpha=0.7, label='O(n^2.81)', linewidth=1.5)
            if any(tag in curves for tag in ["n2", "n^2", "o(n^2)"]):
                plt.plot(theory_sizes, o_n2, '--', color='green', alpha=0.7, label='O(n²)', linewidth=1.5)

        # Configure log-log scales
        plt.xscale('log', base=2)
        plt.yscale('log')

        # Configure x-axis ticks
        plt.xticks(sizes, [str(s) for s in sizes])

        # Labels and title
        plt.xlabel('Matrix Size (n)', fontsize=14)
        plt.ylabel('Time (seconds)', fontsize=14)
        plt.title('Matrix Multiplication Benchmark: Naive vs Strassen\nwith Theoretical Complexity Curves', fontsize=16)

        # Legend and grid
        plt.legend(fontsize=12)
        plt.grid(True, alpha=0.3)

        # Annotations with values
        for i, (size, naive, strassen) in enumerate(results):
            if naive != float('inf'):
                plt.annotate(f"{naive:.3f}", (size, naive),
                            textcoords="offset points", xytext=(0,10), ha='center', fontsize=8)
            if strassen != float('inf'):
                plt.annotate(f"{strassen:.3f}", (size, strassen),
                            textcoords="offset points", xytext=(0,-15), ha='center', fontsize=8)

        plt.tight_layout()

        if filename:
            # Ensure directory exists
            os.makedirs(os.path.dirname(filename), exist_ok=True)
            plt.savefig(filename, dpi=300, bbox_inches='tight')
            print(f"Plot saved to {filename}")

        # Don't show plot in Docker environment
        # plt.show()


def run_test_case(test_case: str, sizes: List[int], num_runs: int = 1):
    """
    Run a specific test case with predefined parameters.
    
    Args:
        test_case: "small", "large", or "random"
        sizes: List of matrix sizes to test
        num_runs: Number of runs per test
    """
    test_configs = {
        "small": {"min_val": -10, "max_val": 10, "suffix": "small_values"},
        "large": {"min_val": -1000, "max_val": 1000, "suffix": "large_values"},
        "random": {"min_val": -100, "max_val": 100, "suffix": "random_values"}
    }
    
    if test_case not in test_configs:
        raise ValueError(f"Unknown test case: {test_case}. Use 'small', 'large', or 'random'")
    
    config = test_configs[test_case]
    
    print(f"\n{'='*60}")
    print(f"Running {test_case.upper()} VALUES benchmark")
    print(f"Matrix values: [{config['min_val']}, {config['max_val']}]")
    print(f"Sizes: {sizes}")
    print(f"Runs per test: {num_runs}")
    print(f"{'='*60}")
    
    # Initialize benchmark
    benchmark = DockerMatMulBenchmark("matmul_executable", cutoff=64)
    
    # Run benchmark
    results = benchmark.run_benchmark(sizes, num_runs, config['min_val'], config['max_val'])
    
    # Save results
    csv_filename = f"results/{config['suffix']}_benchmark.csv"
    plot_filename = f"results/{config['suffix']}_benchmark.png"
    
    benchmark.save_results(results, csv_filename)
    benchmark.plot_results(results, plot_filename)
    
    print(f"\n{test_case.upper()} VALUES benchmark completed!")
    print(f"Results saved to: {csv_filename}")
    print(f"Plot saved to: {plot_filename}")


def main():
    """Main function for command line execution."""
    parser = argparse.ArgumentParser(
        description='Docker-friendly matrix multiplication benchmark',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  %(prog)s --test-case small --runs 1 --max-size 4096
  %(prog)s --test-case large --runs 3 --sizes 2 4 8 16 32 64
  %(prog)s --test-case random --runs 1 --min-size 2 --max-size 1024
        """
    )

    parser.add_argument('--test-case', choices=['small', 'large', 'random'], required=True,
                       help='Test case to run: small values (-10 to 10), large values (-1000 to 1000), or random values (-100 to 100)')

    parser.add_argument('--sizes', nargs='+', type=int,
                       help='List of sizes to test (powers of 2)')

    parser.add_argument('--max-size', type=int, default=4096,
                       help='Maximum size to test (default: 4096)')

    parser.add_argument('--min-size', type=int, default=2,
                       help='Minimum size to test (default: 2)')

    parser.add_argument('--runs', type=int, default=1,
                       help='Number of executions per test (default: 1)')

    parser.add_argument('--cutoff', type=int, default=64,
                       help='Cutoff value for Strassen algorithm (default: 64)')

    args = parser.parse_args()

    # Generate sizes if not provided
    if not args.sizes:
        sizes = []
        size = args.min_size
        while size <= args.max_size:
            sizes.append(size)
            size *= 2
        args.sizes = sizes

    # Validate that all sizes are powers of 2
    for size in args.sizes:
        if size & (size - 1) != 0:
            print(f"Error: {size} is not a power of 2")
            sys.exit(1)

    # Run the specified test case
    run_test_case(args.test_case, args.sizes, args.runs)


if __name__ == '__main__':
    main()