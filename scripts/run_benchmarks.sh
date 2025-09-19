#!/bin/bash

# Unified benchmark runner script
# This script runs matrix multiplication, sorting, and data structures benchmarks

set -e  # Exit on any error

echo "=========================================="
echo "Unified Algorithms & Data Structures Benchmark"
echo "=========================================="
echo "System Information:"
echo "  CPU: $(nproc) cores"
echo "  Memory: $(free -h | grep '^Mem:' | awk '{print $2}')"
echo "  OS: $(uname -a)"
echo "=========================================="

# Create results directory
mkdir -p results

# Default parameters
RUNS=${RUNS:-3}
MATMUL_MAX_SIZE=${MATMUL_MAX_SIZE:-256}
SORTING_MAX_SIZE=${SORTING_MAX_SIZE:-10000}
DS_MAX_SIZE=${DS_MAX_SIZE:-10000}
BENCHMARK_TYPE=${BENCHMARK_TYPE:-all}  # matmul, sorting, datastructures, or all

echo "Benchmark Parameters:"
echo "  Runs per test: $RUNS"
echo "  Matrix mul max size: $MATMUL_MAX_SIZE"
echo "  Sorting max size: $SORTING_MAX_SIZE"
echo "  Data structures max size: $DS_MAX_SIZE"
echo "  Benchmark type: $BENCHMARK_TYPE"
echo "=========================================="

# Function to run matrix multiplication benchmarks
run_matmul_benchmarks() {
    echo ""
    echo "MATRIX MULTIPLICATION BENCHMARKS"
    echo "=================================="

    # Test Case 1: Small Values (-10 to 10)
    echo "Test Case: Small Values (-10 to 10)"
    ./bin/benchmark_matmul --algorithms naive,strassen,blocked,blocked_opt \
                          --max-size $MATMUL_MAX_SIZE --runs $RUNS \
                          --min-val -10 --max-val 10 \
                          --output-csv results/matmul_small_values.csv --quiet

    # Test Case 2: Large Values (-1000 to 1000)
    echo "Test Case: Large Values (-1000 to 1000)"
    ./bin/benchmark_matmul --algorithms naive,strassen,blocked,blocked_opt \
                          --max-size $MATMUL_MAX_SIZE --runs $RUNS \
                          --min-val -1000 --max-val 1000 \
                          --output-csv results/matmul_large_values.csv --quiet

    # Test Case 3: Random Values (-100 to 100)
    echo "Test Case: Random Values (-100 to 100)"
    ./bin/benchmark_matmul --algorithms naive,strassen,blocked,blocked_opt \
                          --max-size $MATMUL_MAX_SIZE --runs $RUNS \
                          --min-val -100 --max-val 100 \
                          --output-csv results/matmul_random_values.csv --quiet
}

# Function to run sorting benchmarks
run_sorting_benchmarks() {
    echo ""
    echo "SORTING ALGORITHMS BENCHMARKS"
    echo "=============================="

    # Test different input patterns
    for pattern in ordered reverse random nearly_sorted; do
        echo "Test Case: $pattern input pattern"
        ./bin/benchmark_sorting --algorithms selection_sort,insertion_sort,quick_sort,merge_sort \
                               --patterns $pattern --max-size $SORTING_MAX_SIZE --runs $RUNS \
                               --output-csv results/sorting_${pattern}.csv --quiet
    done
}

# Function to run data structures benchmarks
run_datastructures_benchmarks() {
    echo ""
    echo "DATA STRUCTURES BENCHMARKS"
    echo "==========================="

    # Test different data structures and operations
    ./bin/benchmark_datastructures --data-structures vector,heap,queue \
                                  --operations push_back,insert,enqueue \
                                  --max-size $DS_MAX_SIZE --runs $RUNS \
                                  --output-csv results/datastructures.csv --quiet
}

# Function to generate plots
generate_plots() {
    echo ""
    echo "GENERATING PLOTS"
    echo "================"

    if [ -f results/matmul_*.csv ]; then
        echo "Generating matrix multiplication plots..."
        python3 scripts/plot_benchmarks.py results/matmul_*.csv \
                --output results/matmul_comparison.png
    fi

    if [ -f results/sorting_*.csv ]; then
        echo "Generating sorting algorithm plots..."
        python3 scripts/plot_benchmarks.py results/sorting_*.csv \
                --output results/sorting_comparison.png
    fi

    if [ -f results/datastructures.csv ]; then
        echo "Generating data structures plots..."
        python3 scripts/plot_benchmarks.py results/datastructures.csv \
                --output results/datastructures_comparison.png
    fi
}

# Main execution
echo "Starting benchmark execution..."

case $BENCHMARK_TYPE in
    matmul)
        run_matmul_benchmarks
        ;;
    sorting)
        run_sorting_benchmarks
        ;;
    datastructures)
        run_datastructures_benchmarks
        ;;
    all)
        run_matmul_benchmarks
        run_sorting_benchmarks
        run_datastructures_benchmarks
        ;;
    *)
        echo "Invalid BENCHMARK_TYPE: $BENCHMARK_TYPE"
        echo "Valid options: matmul, sorting, datastructures, all"
        exit 1
        ;;
esac

# Generate plots if requested
if [ "${GENERATE_PLOTS:-false}" = "true" ]; then
    generate_plots
fi

echo ""
echo "=========================================="
echo "ALL BENCHMARKS COMPLETED!"
echo "=========================================="
echo "Results saved in ./results/ directory:"
ls -la results/
echo "=========================================="

# Print summary
echo ""
echo "SUMMARY:"
# Check if matmul results exist (any file matching pattern)
if ls results/matmul_*.csv >/dev/null 2>&1; then
    echo "- Matrix multiplication results: results/matmul_*.csv"
fi
# Check if sorting results exist (any file matching pattern)
if ls results/sorting_*.csv >/dev/null 2>&1; then
    echo "- Sorting results: results/sorting_*.csv"
fi
if [ -f results/datastructures.csv ]; then
    echo "- Data structures results: results/datastructures.csv"
fi
echo ""
echo "To generate plots manually:"
echo "  python3 scripts/plot_benchmarks.py results/*.csv --output-dir plots/"
echo "=========================================="