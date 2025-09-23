#!/usr/bin/env python3
"""
Plotador para benchmark numérico de sorting (Counting vs Radix+Counting).
Lê CSV gerado por scripts/benchmark_num_sorting.cpp e plota curvas por faixa de valores (max_val).
"""

import argparse
import os
import sys
from typing import List
import pandas as pd
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D  # noqa: F401 - needed for 3D plots
import numpy as np


def load_csv(filename: str) -> pd.DataFrame:
    if not os.path.exists(filename):
        print(f"Erro: arquivo não encontrado: {filename}")
        sys.exit(1)
    try:
        df = pd.read_csv(filename)
        return df
    except Exception as e:
        print(f"Erro ao ler CSV {filename}: {e}")
        sys.exit(1)


def plot_num_sorting(df: pd.DataFrame, output: str = None, use_log_y: bool = True, group_by: str = "auto"):
    required_cols = [
        'size', 'algorithm', 'run_index', 'seed', 'time_s', 'max_val', 'base', 'digits'
    ]
    for c in required_cols:
        if c not in df.columns:
            print(f"Erro: coluna obrigatória ausente no CSV: {c}")
            sys.exit(1)

    # Paleta básica
    colors = {
        'counting_sort': '#1f77b4',
        'radix_sort': '#ff7f0e',
    }

    # Estratégia de agrupamento
    mode = group_by
    if mode not in ("auto", "max_val", "none"):
        mode = "auto"

    if mode == "auto":
        # Se cada max_val tem apenas 1 size distinto (ex.: k = n+1), plot único ignorando max_val
        sizes_per_k = df.groupby('max_val')['size'].nunique()
        unique_sizes = df['size'].nunique()
        unique_k = df['max_val'].nunique()
        if (sizes_per_k.max() <= 1) and (unique_sizes == unique_k):
            mode = "none"
        else:
            mode = "max_val"

    if mode == "max_val":
        grouped = df.groupby(['max_val', 'algorithm', 'size'])['time_s'].mean().reset_index()
        grouped = grouped.sort_values(by=['max_val', 'algorithm', 'size'])

        max_vals = sorted(grouped['max_val'].unique())
        for mv in max_vals:
            fig, ax = plt.subplots(figsize=(12, 8))
            sub = grouped[grouped['max_val'] == mv]
            algorithms = sorted(sub['algorithm'].unique())

            for alg in algorithms:
                s = sub[sub['algorithm'] == alg]
                size_means = s.groupby('size')['time_s'].mean().reset_index()
                ax.plot(
                    size_means['size'].values,
                    size_means['time_s'].values,
                    marker='o', linewidth=2, markersize=5,
                    color=colors.get(alg, None), label=f"{alg}"
                )

            ax.set_title(f"Counting vs Radix (max_val={mv})", fontsize=16, fontweight='bold')
            ax.set_xlabel("Array Size (n)", fontsize=14)
            ax.set_ylabel("Tempo (s)", fontsize=14)
            ax.grid(True, alpha=0.3)
            ax.legend(fontsize=12, loc='upper left')
            if use_log_y:
                ax.set_yscale('log')
            ax.xaxis.set_major_formatter(plt.FuncFormatter(lambda x, p: f'{int(x):,}'))

            if output:
                base_name, ext = os.path.splitext(output)
                out_file = f"{base_name}_max{mv}{ext if ext else '.png'}"
                plt.savefig(out_file, dpi=300, bbox_inches='tight')
                print(f"Figura salva em: {out_file}")

            plt.show()

    else:  # mode == "none" -> ignorar max_val e plotar curvas completas
        fig, ax = plt.subplots(figsize=(12, 8))

        # Se houver 'target_passes', plota uma linha por número de passadas do radix,
        # e uma linha para o counting.
        if 'target_passes' in df.columns and (df['target_passes'].fillna(0).nunique() > 1 or (df['algorithm'] == 'radix_sort').any()):
            # Counting sort (sem passes)
            df_count = df[df['algorithm'] == 'counting_sort']
            if not df_count.empty:
                gc = df_count.groupby(['size'])['time_s'].mean().reset_index().sort_values('size')
                ax.plot(gc['size'].values, gc['time_s'].values,
                        marker='o', linewidth=2, markersize=5,
                        color=colors.get('counting_sort', '#1f77b4'), label='counting_sort')

            # Radix por número de passadas
            dfr = df[df['algorithm'] == 'radix_sort']
            if not dfr.empty:
                passes = sorted(dfr['target_passes'].dropna().unique())
                # mapa de cores
                cmap = plt.get_cmap('tab10')
                for idx, p in enumerate(passes):
                    sub = dfr[dfr['target_passes'] == p]
                    gr = sub.groupby('size')['time_s'].mean().reset_index().sort_values('size')
                    ax.plot(gr['size'].values, gr['time_s'].values,
                            marker='o', linewidth=2, markersize=5,
                            color=cmap(idx % 10), label=f'radix_sort • passes={int(p)}')
        else:
            # Fallback: uma linha por algoritmo
            grouped = df.groupby(['algorithm', 'size'])['time_s'].mean().reset_index()
            grouped = grouped.sort_values(by=['algorithm', 'size'])
            algorithms = sorted(grouped['algorithm'].unique())
            for alg in algorithms:
                s = grouped[grouped['algorithm'] == alg]
                ax.plot(
                    s['size'].values,
                    s['time_s'].values,
                    marker='o', linewidth=2, markersize=5,
                    color=colors.get(alg, None), label=f"{alg}"
                )

        ax.set_title("Counting vs Radix (todas as sizes)", fontsize=16, fontweight='bold')
        ax.set_xlabel("Array Size (n)", fontsize=14)
        ax.set_ylabel("Tempo (s)", fontsize=14)
        ax.grid(True, alpha=0.3)
        ax.legend(fontsize=12, loc='upper left')
        if use_log_y:
            ax.set_yscale('log')
        ax.xaxis.set_major_formatter(plt.FuncFormatter(lambda x, p: f'{int(x):,}'))

        if output:
            plt.savefig(output, dpi=300, bbox_inches='tight')
            print(f"Figura salva em: {output}")

        plt.show()


def plot_radix_surface(df: pd.DataFrame, output: str = None):
    # precisa das colunas target_passes (inteiro) e algorithm == 'radix_sort'
    if 'target_passes' not in df.columns:
        print("Erro: coluna 'target_passes' ausente no CSV. Rode o benchmark com --passes.")
        sys.exit(1)

    dfr = df[df['algorithm'] == 'radix_sort'].copy()
    if dfr.empty:
        print("Sem linhas de radix_sort no CSV.")
        return

    # média por (size, target_passes)
    grp = dfr.groupby(['size', 'target_passes'])['time_s'].mean().reset_index()

    # construir grade
    sizes = sorted(grp['size'].unique())
    passes = sorted(grp['target_passes'].unique())

    X, Y = np.meshgrid(sizes, passes)
    Z = np.zeros_like(X, dtype=float)

    lookup = {(int(r['size']), int(r['target_passes'])): float(r['time_s']) for _, r in grp.iterrows()}
    for i, p in enumerate(passes):
        for j, s in enumerate(sizes):
            Z[i, j] = lookup.get((int(s), int(p)), np.nan)

    fig = plt.figure(figsize=(14, 10))
    ax = fig.add_subplot(111, projection='3d')

    # surface
    surf = ax.plot_surface(X, Y, Z, cmap='viridis', edgecolor='none', alpha=0.9)
    ax.set_title('Radix Sort: Superfície Tempo (n) x Passes', fontsize=16, fontweight='bold')
    ax.set_xlabel('Array Size (n)')
    ax.set_ylabel('Passes (alvo)')
    ax.set_zlabel('Tempo (s)')

    # format eixo X como inteiros com separador
    ax.set_xticks(sizes[::max(1, len(sizes)//10)])
    ax.set_yticks(passes)

    fig.colorbar(surf, shrink=0.6, aspect=12, label='Tempo (s)')

    if output:
        plt.savefig(output, dpi=300, bbox_inches='tight')
        print(f"Figura 3D salva em: {output}")

    plt.show()


def main():
    parser = argparse.ArgumentParser(description='Plot numérico: counting vs radix')
    parser.add_argument('csv', type=str, help='Arquivo CSV de entrada (num_sorting_results.csv)')
    parser.add_argument('--no-log-y', action='store_true', help='Não usar escala log em Y')
    parser.add_argument('--group-by', type=str, default='auto', choices=['auto', 'max_val', 'none'],
                        help='Agrupamento: auto (padrão), max_val (um gráfico por k), none (um gráfico único)')
    parser.add_argument('--output', '-o', type=str, default=None, help='Arquivo base de saída PNG')
    parser.add_argument('--surface', action='store_true', help='Gerar gráfico de superfície 3D para radix (tempo vs size x passes)')
    parser.add_argument('--surface-output', type=str, default=None, help='Arquivo de saída PNG para a superfície 3D')
    args = parser.parse_args()

    df = load_csv(args.csv)

    if args.surface:
        plot_radix_surface(df, args.surface_output)
        return

    plot_num_sorting(df, args.output, use_log_y=not args.no_log_y, group_by=args.group_by)


if __name__ == '__main__':
    main()


