#!/usr/bin/env python3
"""
Benchmark script para comparar matmul_naive vs matmul_strassen
com cutoff de 64, para tamanhos de entrada que são potências de 2.
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


class MatMulBenchmark:
    """Classe para executar benchmarks de multiplicação de matrizes."""

    def __init__(self, executable_path: str, cutoff: int = 64):
        """
        Inicializa o benchmark.

        Args:
            executable_path: Caminho para o executável C++ com as funções de multiplicação
            cutoff: Valor de cutoff para o algoritmo de Strassen
        """
        self.executable_path = executable_path
        self.cutoff = cutoff

    def generate_test_code(self, algorithm: str, size: int, seed: int = 42) -> str:
        """
        Gera código C++ para testar um algoritmo específico.

        Args:
            algorithm: "naive" ou "strassen"
            size: Tamanho da matriz quadrada
            seed: Seed para geração de números aleatórios

        Returns:
            Código C++ como string
        """
        if algorithm == "naive":
            function_call = f"matmul_naive(A, B)"
        elif algorithm == "strassen":
            function_call = f"matmul_strassen(A, B, {self.cutoff})"
        else:
            raise ValueError(f"Algoritmo desconhecido: {algorithm}")

        return f"""
#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include "./algorithms/cpp/luAlgorithms.h"

using namespace std;

static Matrix generate_random_square_matrix(int n, int min_val = -10, int max_val = 10, unsigned seed = {seed}) {{
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
    cout << "Testando {algorithm} com n=" << n << endl;

    // Gera matrizes de teste
    Matrix A = generate_random_square_matrix(n, -10, 10, 123);
    Matrix B = generate_random_square_matrix(n, -10, 10, 456);

    // Marca tempo inicial
    auto start = chrono::high_resolution_clock::now();

    // Executa multiplicação
    Matrix C = {function_call};

    // Marca tempo final
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;

    cout << "Tempo: " << duration.count() << " segundos" << endl;
    cout << "Tamanho resultado: " << C.size() << "x" << C[0].size() << endl;

    return 0;
}}
"""

    def run_single_test(self, algorithm: str, size: int, seed: int = 42) -> float:
        """
        Executa um único teste de benchmark.

        Args:
            algorithm: Algoritmo a testar ("naive" ou "strassen")
            size: Tamanho da matriz
            seed: Seed para reprodutibilidade

        Returns:
            Tempo de execução em segundos
        """
        # Gera código de teste
        test_code = self.generate_test_code(algorithm, size, seed)

        # Cria arquivo temporário
        temp_filename = f"temp_benchmark_{algorithm}_{size}.cpp"
        with open(temp_filename, 'w') as f:
            f.write(test_code)

        try:
            # Compila o código
            compile_cmd = f"g++ -std=c++17 -O3 -I. {temp_filename} ./algorithms/cpp/luAlgorithms.cpp -o temp_benchmark_{algorithm}_{size}"
            result = subprocess.run(compile_cmd, shell=True, capture_output=True, text=True)

            if result.returncode != 0:
                print(f"Erro na compilação para {algorithm} n={size}:")
                print(result.stderr)
                return float('inf')

            # Executa o benchmark
            run_cmd = f"./temp_benchmark_{algorithm}_{size}"
            result = subprocess.run(run_cmd, shell=True, capture_output=True, text=True)

            if result.returncode != 0:
                print(f"Erro na execução para {algorithm} n={size}:")
                print(result.stderr)
                return float('inf')

            # Extrai o tempo da saída
            output = result.stdout
            for line in output.split('\n'):
                if line.startswith('Tempo:'):
                    time_str = line.split(':')[1].strip().split()[0]
                    return float(time_str)

            print(f"Não foi possível extrair tempo da saída para {algorithm} n={size}")
            return float('inf')

        except Exception as e:
            print(f"Erro executando teste {algorithm} n={size}: {e}")
            return float('inf')

        finally:
            # Limpa arquivos temporários
            for ext in ['.cpp', '']:
                temp_file = f"temp_benchmark_{algorithm}_{size}{ext}"
                if os.path.exists(temp_file):
                    os.remove(temp_file)

    def run_benchmark(self, sizes: List[int], num_runs: int = 3) -> List[Tuple[int, float, float]]:
        """
        Executa benchmarks completos para todos os tamanhos.

        Args:
            sizes: Lista de tamanhos a testar
            num_runs: Número de execuções por teste para média

        Returns:
            Lista de tuplas (size, naive_time, strassen_time)
        """
        results = []

        print(f"Iniciando benchmark com {len(sizes)} tamanhos e {num_runs} execuções por teste")
        print("=" * 60)

        for size in sizes:
            print(f"\nTestando n={size}")

            naive_times = []
            strassen_times = []

            # Executa múltiplas vezes para cada algoritmo
            for run in range(num_runs):
                print(f"  Run {run + 1}/{num_runs}...")

                # Testa naive
                naive_time = self.run_single_test("naive", size, seed=42 + run)
                if naive_time != float('inf'):
                    naive_times.append(naive_time)

                # Testa strassen
                strassen_time = self.run_single_test("strassen", size, seed=42 + run)
                if strassen_time != float('inf'):
                    strassen_times.append(strassen_time)

            # Calcula médias
            avg_naive = sum(naive_times) / len(naive_times) if naive_times else float('inf')
            avg_strassen = sum(strassen_times) / len(strassen_times) if strassen_times else float('inf')

            results.append((size, avg_naive, avg_strassen))
            print(f"  Naive: {avg_naive:.6f}s")
            print(f"  Strassen: {avg_strassen:.6f}s")

        return results

    def save_results(self, results: List[Tuple[int, float, float]], filename: str):
        """
        Salva os resultados em arquivo CSV.

        Args:
            results: Lista de resultados (size, naive_time, strassen_time)
            filename: Nome do arquivo CSV
        """
        with open(filename, 'w', newline='') as csvfile:
            writer = csv.writer(csvfile)
            writer.writerow(['size', 'naive_time', 'strassen_time'])
            for size, naive_time, strassen_time in results:
                writer.writerow([size, naive_time, strassen_time])

        print(f"Resultados salvos em {filename}")

    def plot_results(self, results: List[Tuple[int, float, float]], filename: str = None, theory_curves: List[str] = None):
        """
        Gera gráfico comparativo dos resultados.

        Args:
            results: Lista de resultados (size, naive_time, strassen_time)
            filename: Nome do arquivo para salvar o gráfico (opcional)
            theory_curves: Lista com curvas teóricas a exibir. Valores:
                ["n3", "n2.81", "n2"]. Padrão: todas.
        """
        sizes = [r[0] for r in results]
        naive_times = [r[1] for r in results]
        strassen_times = [r[2] for r in results]

        plt.figure(figsize=(12, 8))

        # Plota os tempos experimentais
        plt.plot(sizes, naive_times, 'o-', label='Naive', linewidth=2, markersize=6)
        plt.plot(sizes, strassen_times, 's-', label='Strassen (cutoff=64)', linewidth=2, markersize=6)

        # Adiciona curvas teóricas de complexidade (baseline comum)
        if sizes:
            # Pega o menor tamanho como referência
            min_size = min(sizes)
            min_idx = sizes.index(min_size)

            # Tempos em n mínimo
            naive_min_n = naive_times[min_idx]
            strassen_min_n = strassen_times[min_idx]

            # Baseline comum: média dos disponíveis em n mínimo (fallbacks seguros)
            base_candidates = [t for t in [naive_min_n, strassen_min_n] if t != float('inf')]
            if len(base_candidates) == 0:
                base_candidates = [t for t in (naive_times + strassen_times) if t != float('inf')]
            if len(base_candidates) == 0:
                base_time = 1.0
            else:
                base_time = sum(base_candidates) / len(base_candidates)

            # Gera pontos para as curvas teóricas
            theory_sizes = np.linspace(min_size, max(sizes), 100)

            # Curvas teóricas normalizadas no mesmo ponto base
            o_n3 = base_time * (theory_sizes / min_size) ** 3.0
            o_n281 = base_time * (theory_sizes / min_size) ** 2.807
            o_n2 = base_time * (theory_sizes / min_size) ** 2.0

            # Seleção de curvas teóricas
            curves = set(c.lower() for c in (theory_curves or ["n3", "n2.81", "n2"]))
            if any(tag in curves for tag in ["n3", "n^3", "o(n^3)", "o(n3)"]):
                plt.plot(theory_sizes, o_n3, '--', color='blue', alpha=0.7, label='O(n³)', linewidth=1.5)
            if any(tag in curves for tag in ["n2.81", "n^2.81", "o(n^2.81)"]):
                plt.plot(theory_sizes, o_n281, '--', color='red', alpha=0.7, label='O(n^2.81)', linewidth=1.5)
            if any(tag in curves for tag in ["n2", "n^2", "o(n^2)"]):
                plt.plot(theory_sizes, o_n2, '--', color='green', alpha=0.7, label='O(n²)', linewidth=1.5)

        # Configura escalas log-log
        plt.xscale('log', base=2)
        plt.yscale('log')

        # Configura ticks do eixo x para mostrar valores reais
        plt.xticks(sizes, [str(s) for s in sizes])

        # Labels e título
        plt.xlabel('Tamanho da Matriz (n)', fontsize=14)
        plt.ylabel('Tempo (segundos)', fontsize=14)
        plt.title('Comparação: matmul_naive vs matmul_strassen\ncom curvas de complexidade teórica', fontsize=16)

        # Legenda e grid
        plt.legend(fontsize=12)
        plt.grid(True, alpha=0.3)

        # Anotações com valores
        for i, (size, naive, strassen) in enumerate(results):
            if naive != float('inf'):
                plt.annotate(f"{naive:.3f}", (size, naive),
                            textcoords="offset points", xytext=(0,10), ha='center', fontsize=8)
            if strassen != float('inf'):
                plt.annotate(f"{strassen:.3f}", (size, strassen),
                            textcoords="offset points", xytext=(0,-15), ha='center', fontsize=8)

        plt.tight_layout()

        if filename:
            plt.savefig(filename, dpi=300, bbox_inches='tight')
            print(f"Gráfico salvo em {filename}")

        plt.show()

    def load_results_from_csv(self, filename: str) -> List[Tuple[int, float, float]]:
        """Carrega resultados (size, naive_time, strassen_time) de um CSV previamente salvo."""
        rows: List[Tuple[int, float, float]] = []
        if not os.path.exists(filename):
            print(f"Arquivo não encontrado: {filename}")
            return rows
        with open(filename, 'r') as f:
            reader = csv.reader(f)
            header = next(reader, None)
            for row in reader:
                try:
                    size = int(row[0])
                    naive_time = float(row[1])
                    strassen_time = float(row[2])
                    rows.append((size, naive_time, strassen_time))
                except Exception:
                    continue
        return rows


def main():
    """Função principal para executar via linha de comando."""
    parser = argparse.ArgumentParser(
        description='Benchmark para comparar matmul_naive vs matmul_strassen',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Exemplos de uso:
  %(prog)s --sizes 2 4 8 16 32 64 128 256 512 1024 --runs 5
  %(prog)s --max-size 2048 --runs 3
  %(prog)s --sizes 64 128 256 512 1024 2048 4096 8192 --output meus_resultados.csv
        """
    )

    parser.add_argument('--sizes', nargs='+', type=int,
                       help='Lista de tamanhos a testar (potências de 2)')

    parser.add_argument('--max-size', type=int, default=8192,
                       help='Tamanho máximo a testar (padrão: 8192)')

    parser.add_argument('--min-size', type=int, default=2,
                       help='Tamanho mínimo a testar (padrão: 2)')

    parser.add_argument('--runs', type=int, default=3,
                       help='Número de execuções por teste (padrão: 3)')

    parser.add_argument('--output', type=str, default='matmul_benchmark_results.csv',
                       help='Arquivo CSV para salvar resultados')

    parser.add_argument('--plot', type=str, default='matmul_benchmark_plot.png',
                       help='Arquivo PNG para salvar gráfico')

    parser.add_argument('--cutoff', type=int, default=64,
                       help='Valor de cutoff para Strassen (padrão: 64)')

    parser.add_argument('--replot-from', type=str, default=None,
                       help='Regera a figura lendo resultados de um CSV (não roda benchmark)')

    parser.add_argument('--theory', type=str, default='n3,n2.81,n2',
                       help='Curvas teóricas a exibir, separadas por vírgula. Opções: n3,n2.81,n2')

    args = parser.parse_args()

    # Interpreta curvas teóricas
    theory_curves = [s.strip().lower() for s in args.theory.split(',') if s.strip()]

    # Modo replot: ler CSV e apenas plotar
    if args.replot_from:
        print("Replot a partir do CSV existente (sem rodar benchmark)")
        benchmark = MatMulBenchmark("matmul_executable", args.cutoff)
        results = benchmark.load_results_from_csv(args.replot_from)
        if not results:
            print("Nenhum resultado encontrado no CSV informado.")
            sys.exit(1)
        # Ordena por tamanho, caso necessário
        results = sorted(results, key=lambda r: r[0])
        benchmark.plot_results(results, args.plot, theory_curves=theory_curves)
        return

    # Valida argumentos (apenas se não for replot)
    if not args.sizes:
        # Gera potências de 2 automaticamente
        sizes = []
        size = args.min_size
        while size <= args.max_size:
            sizes.append(size)
            size *= 2
        args.sizes = sizes

    # Valida que todos os tamanhos são potências de 2
    for size in args.sizes:
        if size & (size - 1) != 0:
            print(f"Erro: {size} não é uma potência de 2")
            sys.exit(1)

    print("Configuração do benchmark:")
    print(f"  Tamanhos: {args.sizes}")
    print(f"  Execuções por teste: {args.runs}")
    print(f"  Cutoff Strassen: {args.cutoff}")
    print(f"  Arquivo de saída: {args.output}")
    print(f"  Arquivo de gráfico: {args.plot}")

    # Inicializa benchmark
    benchmark = MatMulBenchmark("matmul_executable", args.cutoff)

    # Executa benchmark
    results = benchmark.run_benchmark(args.sizes, args.runs)

    # Salva resultados
    benchmark.save_results(results, args.output)

    # Gera gráfico
    benchmark.plot_results(results, args.plot, theory_curves=theory_curves)


if __name__ == '__main__':
    main()
