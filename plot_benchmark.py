#!/usr/bin/env python3
"""
Script para plotagem de resultados de benchmark de multiplicação de matrizes.
Lê dados de arquivos CSV ou JSON gerados pelo benchmark C++.
"""

import argparse
import csv
import json
import os
import sys
from typing import List, Tuple, Dict, Any
import matplotlib.pyplot as plt
import numpy as np


class BenchmarkPlotter:
    """Classe para plotagem de resultados de benchmark."""
    
    def __init__(self):
        self.results = []
        self.metadata = {}
    
    def load_csv(self, filename: str) -> bool:
        """
        Carrega resultados de um arquivo CSV.
        
        Args:
            filename: Caminho para o arquivo CSV
            
        Returns:
            True se carregou com sucesso, False caso contrário
        """
        if not os.path.exists(filename):
            print(f"Arquivo não encontrado: {filename}")
            return False
        
        try:
            with open(filename, 'r') as f:
                reader = csv.DictReader(f)
                self.results = []
                
                for row in reader:
                    result = {
                        'size': int(row['size']),
                        'naive_time': float(row['naive_time']),
                        'strassen_time': float(row['strassen_time']),
                        'num_runs': int(row['num_runs']),
                        'cutoff': int(row['cutoff']),
                        'min_val': int(row['min_val']),
                        'max_val': int(row['max_val'])
                    }
                    self.results.append(result)
                
                # Extrai metadados do primeiro resultado
                if self.results:
                    first = self.results[0]
                    self.metadata = {
                        'cutoff': first['cutoff'],
                        'min_val': first['min_val'],
                        'max_val': first['max_val'],
                        'num_runs': first['num_runs']
                    }
                
                print(f"Carregados {len(self.results)} resultados de {filename}")
                return True
                
        except Exception as e:
            print(f"Erro ao carregar CSV {filename}: {e}")
            return False
    
    def load_json(self, filename: str) -> bool:
        """
        Carrega resultados de um arquivo JSON.
        
        Args:
            filename: Caminho para o arquivo JSON
            
        Returns:
            True se carregou com sucesso, False caso contrário
        """
        if not os.path.exists(filename):
            print(f"Arquivo não encontrado: {filename}")
            return False
        
        try:
            with open(filename, 'r') as f:
                data = json.load(f)
                
            self.metadata = data.get('metadata', {})
            self.results = data.get('results', [])
            
            print(f"Carregados {len(self.results)} resultados de {filename}")
            return True
            
        except Exception as e:
            print(f"Erro ao carregar JSON {filename}: {e}")
            return False
    
    def get_sizes_and_times(self) -> Tuple[List[int], List[float], List[float]]:
        """
        Extrai listas de tamanhos e tempos dos resultados.
        
        Returns:
            Tupla com (sizes, naive_times, strassen_times)
        """
        sizes = [r['size'] for r in self.results]
        naive_times = [r['naive_time'] for r in self.results]
        strassen_times = [r['strassen_time'] for r in self.results]
        
        return sizes, naive_times, strassen_times
    
    def plot_comparison(self, output_file: str = None, theory_curves: List[str] = None, 
                       title: str = None, show_annotations: bool = True) -> None:
        """
        Gera gráfico de comparação dos algoritmos.
        
        Args:
            output_file: Arquivo para salvar o gráfico (opcional)
            theory_curves: Lista de curvas teóricas a exibir
            title: Título personalizado do gráfico
            show_annotations: Se deve mostrar anotações com valores
        """
        if not self.results:
            print("Nenhum resultado carregado para plotar")
            return
        
        sizes, naive_times, strassen_times = self.get_sizes_and_times()
        
        plt.figure(figsize=(12, 8))
        
        # Plota os tempos experimentais
        plt.plot(sizes, naive_times, 'o-', label='Naive', linewidth=2, markersize=6, color='blue')
        plt.plot(sizes, strassen_times, 's-', 
                f'Strassen (cutoff={self.metadata.get("cutoff", "?")})', 
                linewidth=2, markersize=6, color='red')
        
        # Adiciona curvas teóricas
        if theory_curves and sizes:
            self._add_theory_curves(sizes, naive_times, strassen_times, theory_curves)
        
        # Configurações do gráfico
        plt.xscale('log', base=2)
        plt.yscale('log')
        plt.xticks(sizes, [str(s) for s in sizes])
        
        # Labels e título
        plt.xlabel('Tamanho da Matriz (n)', fontsize=14)
        plt.ylabel('Tempo (segundos)', fontsize=14)
        
        if title:
            plt.title(title, fontsize=16)
        else:
            cutoff = self.metadata.get('cutoff', '?')
            min_val = self.metadata.get('min_val', '?')
            max_val = self.metadata.get('max_val', '?')
            plt.title(f'Comparação: matmul_naive vs matmul_strassen\\n'
                     f'cutoff={cutoff}, valores=[{min_val}, {max_val}]', fontsize=16)
        
        # Anotações com valores
        if show_annotations:
            for i, (size, naive, strassen) in enumerate(zip(sizes, naive_times, strassen_times)):
                if naive != float('inf'):
                    plt.annotate(f"{naive:.3f}", (size, naive),
                               textcoords="offset points", xytext=(0,10), ha='center', fontsize=8)
                if strassen != float('inf'):
                    plt.annotate(f"{strassen:.3f}", (size, strassen),
                               textcoords="offset points", xytext=(0,-15), ha='center', fontsize=8)
        
        plt.legend(fontsize=12)
        plt.grid(True, alpha=0.3)
        plt.tight_layout()
        
        if output_file:
            plt.savefig(output_file, dpi=300, bbox_inches='tight')
            print(f"Gráfico salvo em {output_file}")
        
        plt.show()
    
    def _add_theory_curves(self, sizes: List[int], naive_times: List[float], 
                          strassen_times: List[float], theory_curves: List[str]) -> None:
        """Adiciona curvas teóricas ao gráfico."""
        min_size = min(sizes)
        min_idx = sizes.index(min_size)
        
        # Calcula baseline
        base_candidates = [t for t in [naive_times[min_idx], strassen_times[min_idx]] 
                          if t != float('inf')]
        if not base_candidates:
            base_candidates = [t for t in (naive_times + strassen_times) if t != float('inf')]
        base_time = sum(base_candidates) / len(base_candidates) if base_candidates else 1.0
        
        # Pontos para curvas teóricas
        theory_sizes = np.linspace(min_size, max(sizes), 100)
        
        # Mapeia nomes de curvas
        curve_configs = {
            'n3': (3.0, 'O(n³)', 'blue', '--'),
            'n^3': (3.0, 'O(n³)', 'blue', '--'),
            'n2.81': (2.807, 'O(n^2.81)', 'red', '--'),
            'n^2.81': (2.807, 'O(n^2.81)', 'red', '--'),
            'n2': (2.0, 'O(n²)', 'green', '--'),
            'n^2': (2.0, 'O(n²)', 'green', '--'),
        }
        
        for curve in theory_curves:
            curve_lower = curve.lower().strip()
            if curve_lower in curve_configs:
                exponent, label, color, style = curve_configs[curve_lower]
                theory_times = base_time * (theory_sizes / min_size) ** exponent
                plt.plot(theory_sizes, theory_times, style, color=color, 
                        alpha=0.7, label=label, linewidth=1.5)
    
    def print_summary(self) -> None:
        """Imprime um resumo dos resultados."""
        if not self.results:
            print("Nenhum resultado carregado")
            return
        
        print("\\n=== Resumo dos Resultados ===")
        print(f"Número de testes: {len(self.results)}")
        print(f"Cutoff: {self.metadata.get('cutoff', 'N/A')}")
        print(f"Valores das matrizes: [{self.metadata.get('min_val', 'N/A')}, {self.metadata.get('max_val', 'N/A')}]")
        print(f"Execuções por teste: {self.metadata.get('num_runs', 'N/A')}")
        
        sizes, naive_times, strassen_times = self.get_sizes_and_times()
        
        print("\\nTamanho | Naive (s) | Strassen (s) | Speedup")
        print("-" * 45)
        for size, naive, strassen in zip(sizes, naive_times, strassen_times):
            speedup = naive / strassen if strassen > 0 and strassen != float('inf') else float('inf')
            speedup_str = f"{speedup:.2f}x" if speedup != float('inf') else "∞"
            print(f"{size:7d} | {naive:9.6f} | {strassen:12.6f} | {speedup_str}")


def main():
    """Função principal."""
    parser = argparse.ArgumentParser(
        description='Plota resultados de benchmark de multiplicação de matrizes',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Exemplos de uso:
%(prog)s benchmark_results.csv
%(prog)s --json results.json --output grafico.png
%(prog)s data.csv --theory n3,n2.81 --title "Meu Benchmark"
%(prog)s --csv data1.csv --json data2.json  # Combina múltiplos arquivos
        """
    )
    
    parser.add_argument('files', nargs='*', 
                       help='Arquivos CSV ou JSON com resultados (detecta formato pela extensão)')
    
    parser.add_argument('--csv', type=str, action='append',
                       help='Arquivo CSV específico (pode usar múltiplas vezes)')
    
    parser.add_argument('--json', type=str, action='append', 
                       help='Arquivo JSON específico (pode usar múltiplas vezes)')
    
    parser.add_argument('--output', type=str, default=None,
                       help='Arquivo para salvar o gráfico (ex: plot.png)')
    
    parser.add_argument('--theory', type=str, default='n3,n2.81,n2',
                       help='Curvas teóricas separadas por vírgula (n3,n2.81,n2)')
    
    parser.add_argument('--title', type=str, default=None,
                       help='Título personalizado do gráfico')
    
    parser.add_argument('--no-annotations', action='store_true',
                       help='Não mostra anotações com valores no gráfico')
    
    parser.add_argument('--summary-only', action='store_true',
                       help='Apenas mostra resumo, sem plotar')
    
    args = parser.parse_args()
    
    # Coleta arquivos de entrada
    input_files = []
    
    # Arquivos posicionais
    for file in args.files:
        if file.endswith('.json'):
            input_files.append(('json', file))
        elif file.endswith('.csv'):
            input_files.append(('csv', file))
        else:
            print(f"Formato desconhecido para {file}, tentando CSV...")
            input_files.append(('csv', file))
    
    # Arquivos específicos
    if args.csv:
        for file in args.csv:
            input_files.append(('csv', file))
    
    if args.json:
        for file in args.json:
            input_files.append(('json', file))
    
    if not input_files:
        print("Erro: Nenhum arquivo de entrada especificado")
        parser.print_help()
        sys.exit(1)
    
    # Carrega dados
    plotter = BenchmarkPlotter()
    all_results = []
    
    for file_type, filename in input_files:
        temp_plotter = BenchmarkPlotter()
        
        if file_type == 'csv':
            success = temp_plotter.load_csv(filename)
        else:
            success = temp_plotter.load_json(filename)
        
        if success:
            all_results.extend(temp_plotter.results)
            # Usa metadados do primeiro arquivo bem-sucedido
            if not plotter.metadata:
                plotter.metadata = temp_plotter.metadata
    
    if not all_results:
        print("Erro: Nenhum dado foi carregado com sucesso")
        sys.exit(1)
    
    # Remove duplicatas e ordena por tamanho
    seen = set()
    unique_results = []
    for result in all_results:
        key = result['size']
        if key not in seen:
            seen.add(key)
            unique_results.append(result)
    
    plotter.results = sorted(unique_results, key=lambda x: x['size'])
    
    # Mostra resumo
    plotter.print_summary()
    
    if args.summary_only:
        return
    
    # Interpreta curvas teóricas
    theory_curves = [s.strip() for s in args.theory.split(',') if s.strip()]
    
    # Gera gráfico
    plotter.plot_comparison(
        output_file=args.output,
        theory_curves=theory_curves,
        title=args.title,
        show_annotations=not args.no_annotations
    )


if __name__ == '__main__':
    main()
