#!/usr/bin/env python3
"""
Gera gráficos a partir da saída CSV do benchmark_quicksort.

Uso:
  python scripts/plot_quicksort.py <csv_entrada> -o <png_saida>

CSV esperado (cabeçalhos):
  pivot,level,size,time_ms,status
"""

import argparse
import os
import sys
from typing import List
import pandas as pd
import matplotlib.pyplot as plt


def load_csv(filename: str) -> pd.DataFrame:
    if not os.path.exists(filename):
        print(f"Erro: arquivo não encontrado: {filename}")
        sys.exit(1)
    try:
        # lê, ignorando linhas de ruído (sem vírgulas suficientes ou cabeçalhos repetidos)
        lines = []
        with open(filename, 'r', encoding='utf-8') as f:
            for line in f:
                s = line.strip()
                if not s:
                    continue
                # mantém header uma vez
                if s.startswith('pivot,level,size,time_ms,status'):
                    if not lines:
                        lines.append(s)
                    continue
                # mantém apenas linhas com 4 vírgulas (5 colunas)
                if s.count(',') == 4:
                    lines.append(s)
        from io import StringIO
        df = pd.read_csv(StringIO('\n'.join(lines)))
        return df
    except Exception as e:
        print(f"Erro ao ler CSV {filename}: {e}")
        sys.exit(1)


def plot_quicksort(df: pd.DataFrame, output: str = None, use_log_y: bool = True):
    required_cols = ['pivot', 'level', 'size', 'time_ms', 'status']
    for c in required_cols:
        if c not in df.columns:
            print(f"Erro: coluna obrigatória ausente no CSV: {c}")
            sys.exit(1)

    # Converte tipos
    df['level'] = df['level'].astype(int)
    df['size'] = df['size'].astype(int)
    df['time_ms'] = df['time_ms'].astype(float)

    # Paleta por pivot
    colors = {
        'last': '#1f77b4',
        'random': '#ff7f0e',
        'median3': '#2ca02c',
    }

    # Uma figura por nível de distribuição
    levels = sorted(df['level'].unique())
    for lvl in levels:
        sub = df[df['level'] == lvl].copy()
        if sub.empty:
            continue

        fig, ax = plt.subplots(figsize=(12, 8))

        for pivot in sorted(sub['pivot'].unique()):
            s = sub[sub['pivot'] == pivot].copy()
            s = s.groupby('size', as_index=False)['time_ms'].mean().sort_values('size')
            ax.plot(
                s['size'].values,
                s['time_ms'].values,
                marker='o', linewidth=2, markersize=5,
                color=colors.get(pivot, None), label=f"{pivot}"
            )

        ax.set_title(f"Quicksort vs Quicksort Aleatório/Mediana (nível={lvl})", fontsize=16, fontweight='bold')
        ax.set_xlabel("Tamanho do vetor (n)", fontsize=14)
        ax.set_ylabel("Tempo (ms)", fontsize=14)
        ax.grid(True, alpha=0.3)
        ax.legend(fontsize=12, loc='upper left')
        if use_log_y:
            ax.set_yscale('log')
        ax.xaxis.set_major_formatter(plt.FuncFormatter(lambda x, p: f'{int(x):,}'))

        if output:
            base, ext = os.path.splitext(output)
            out_file = f"{base}_level{lvl}{ext if ext else '.png'}"
            plt.savefig(out_file, dpi=300, bbox_inches='tight')
            print(f"Figura salva em: {out_file}")

        plt.show()


def main():
    parser = argparse.ArgumentParser(description='Plot para benchmark_quicksort (CSV).')
    parser.add_argument('csv', type=str, help='Arquivo CSV gerado com --output csv')
    parser.add_argument('--no-log-y', action='store_true', help='Não usar escala log em Y')
    parser.add_argument('--output', '-o', type=str, default=None, help='Arquivo base de saída PNG (uma figura por nível)')
    args = parser.parse_args()

    df = load_csv(args.csv)
    plot_quicksort(df, args.output, use_log_y=not args.no_log_y)


if __name__ == '__main__':
    main()


