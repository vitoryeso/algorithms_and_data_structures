#!/usr/bin/env python3
"""
Unified benchmark plotting script for algorithms and data structures.
Supports matmul, sorting, and data structures benchmarks.
Reads CSV/JSON files with standardized schema and generates comparative plots.
"""

import argparse
import csv
import json
import os
import sys
from typing import List, Tuple, Dict, Any, Optional
import matplotlib.pyplot as plt
import matplotlib.colors as mcolors
import numpy as np
import pandas as pd
from pathlib import Path


class ColorScheme:
    """Professional color scheme for benchmark plots."""

    # Professional color palette
    ALGORITHMS = [
        '#1f77b4', '#ff7f0e', '#2ca02c', '#d62728', '#9467bd',
        '#8c564b', '#e377c2', '#7f7f7f', '#bcbd22', '#17becf'
    ]

    DATA_STRUCTURES = [
        '#4e79a7', '#f28e2c', '#e15759', '#76b7b2', '#59a14f',
        '#edc949', '#af7aa1', '#ff9da7', '#9c755f', '#bab0ab'
    ]

    PATTERNS = [
        '#1b9e77', '#d95f02', '#7570b3', '#e7298a', '#66a61e',
        '#e6ab02', '#a6761d', '#666666', '#1f78b4', '#33a02c'
    ]

    OPERATIONS = [
        '#e41a1c', '#377eb8', '#4daf4a', '#984ea3', '#ff7f00',
        '#ffff33', '#a65628', '#f781bf', '#999999', '#66c2a5'
    ]

    @classmethod
    def get_algorithm_color(cls, index):
        """Get color for algorithm by index."""
        return cls.ALGORITHMS[index % len(cls.ALGORITHMS)]

    @classmethod
    def get_structure_color(cls, index):
        """Get color for data structure by index."""
        return cls.DATA_STRUCTURES[index % len(cls.DATA_STRUCTURES)]

    @classmethod
    def get_pattern_color(cls, index):
        """Get color for input pattern by index."""
        return cls.PATTERNS[index % len(cls.PATTERNS)]

    @classmethod
    def get_operation_color(cls, index):
        """Get color for operation by index."""
        return cls.OPERATIONS[index % len(cls.OPERATIONS)]


class BenchmarkData:
    """Container for benchmark data with automatic type detection."""

    def __init__(self, csv_files: List[str] = None, json_files: List[str] = None):
        self.dataframes = []
        self.metadata = {}
        self.benchmark_types = set()  # Support multiple types
        self.config_params = {}  # Store configuration parameters

        if csv_files:
            for file in csv_files:
                df, meta = self._load_csv(file)
                if df is not None:
                    self.dataframes.append(df)
                    self.metadata.update(meta)

        if json_files:
            for file in json_files:
                df, meta = self._load_json(file)
                if df is not None:
                    self.dataframes.append(df)
                    self.metadata.update(meta)

        # Detect benchmark types and extract config parameters
        if self.dataframes:
            self._detect_benchmark_types()
            self._extract_config_parameters()

    def _load_csv(self, filename: str) -> Tuple[Optional[pd.DataFrame], Dict]:
        """Load data from CSV file."""
        if not os.path.exists(filename):
            print(f"Warning: CSV file not found: {filename}")
            return None, {}

        try:
            df = pd.read_csv(filename)
            metadata = {}

            # Extract metadata from columns if present
            meta_cols = ['cpu_model', 'cpu_cores', 'compiler_version', 'os_info', 'timestamp', 'hostname']
            for col in meta_cols:
                if col in df.columns:
                    metadata[col] = df[col].iloc[0] if not df[col].empty else "Unknown"
                    df = df.drop(columns=[col])

            return df, metadata
        except Exception as e:
            print(f"Error loading CSV {filename}: {e}")
            return None, {}

    def _load_json(self, filename: str) -> Tuple[Optional[pd.DataFrame], Dict]:
        """Load data from JSON file."""
        if not os.path.exists(filename):
            print(f"Warning: JSON file not found: {filename}")
            return None, {}

        try:
            with open(filename, 'r') as f:
                data = json.load(f)

            metadata = data.get('metadata', {})
            results = data.get('results', [])

            if not results:
                return None, metadata

            df = pd.DataFrame(results)
            return df, metadata
        except Exception as e:
            print(f"Error loading JSON {filename}: {e}")
            return None, {}

    def _detect_benchmark_types(self):
        """Detect all benchmark types present in the data."""
        if not self.dataframes:
            return

        # Check each dataframe for benchmark types
        for df in self.dataframes:
            # Matmul benchmark detection
            if 'algorithm' in df.columns and 'time_s' in df.columns:
                if 'cutoff' in df.columns or 'block_size' in df.columns:
                    self.benchmark_types.add('matmul')
                elif 'input_pattern' in df.columns:
                    self.benchmark_types.add('sorting')
                else:
                    self.benchmark_types.add('matmul')  # Default for algorithm + time_s

            if 'algorithm' in df.columns and 'input_pattern' in df.columns:
                self.benchmark_types.add('sorting')

            if 'data_structure' in df.columns and 'operation' in df.columns:
                self.benchmark_types.add('datastructures')

            # Additional detection based on specific columns
            if 'cutoff' in df.columns or 'block_size' in df.columns:
                self.benchmark_types.add('matmul')
            if 'input_pattern' in df.columns:
                self.benchmark_types.add('sorting')
            if 'operation' in df.columns:
                self.benchmark_types.add('datastructures')

        # Set primary type for backward compatibility
        if self.benchmark_types:
            if 'matmul' in self.benchmark_types:
                self.benchmark_type = 'matmul'
            elif 'sorting' in self.benchmark_types:
                self.benchmark_type = 'sorting'
            elif 'datastructures' in self.benchmark_types:
                self.benchmark_type = 'datastructures'
            else:
                self.benchmark_type = 'unknown'
        else:
            self.benchmark_type = 'unknown'

    def _extract_config_parameters(self):
        """Extract configuration parameters from the data."""
        if not self.dataframes:
            return

        # Combine all dataframes to extract parameters
        combined_df = pd.concat(self.dataframes, ignore_index=True)

        # Extract matmul parameters
        if 'matmul' in self.benchmark_types:
            matmul_params = {}
            if 'cutoff' in combined_df.columns:
                matmul_params['cutoff'] = combined_df['cutoff'].unique().tolist()
            if 'block_size' in combined_df.columns:
                matmul_params['block_size'] = combined_df['block_size'].unique().tolist()
            if 'algorithm' in combined_df.columns:
                matmul_params['algorithms'] = combined_df['algorithm'].unique().tolist()
            if 'size' in combined_df.columns:
                matmul_params['sizes'] = sorted(combined_df['size'].unique().tolist())
            if 'run_index' in combined_df.columns:
                matmul_params['runs_per_test'] = len(combined_df['run_index'].unique())
            if 'seed' in combined_df.columns:
                matmul_params['seeds'] = combined_df['seed'].unique().tolist()
            if 'min_val' in combined_df.columns:
                matmul_params['min_val'] = combined_df['min_val'].unique().tolist()
            if 'max_val' in combined_df.columns:
                matmul_params['max_val'] = combined_df['max_val'].unique().tolist()

            self.config_params['matmul'] = matmul_params

        # Extract sorting parameters
        if 'sorting' in self.benchmark_types:
            sorting_params = {}
            if 'algorithm' in combined_df.columns:
                sorting_params['algorithms'] = combined_df['algorithm'].unique().tolist()
            if 'input_pattern' in combined_df.columns:
                sorting_params['input_patterns'] = combined_df['input_pattern'].unique().tolist()
            if 'size' in combined_df.columns:
                sorting_params['sizes'] = sorted(combined_df['size'].unique().tolist())
            if 'run_index' in combined_df.columns:
                sorting_params['runs_per_test'] = len(combined_df['run_index'].unique())
            if 'seed' in combined_df.columns:
                sorting_params['seeds'] = combined_df['seed'].unique().tolist()
            if 'min_val' in combined_df.columns:
                sorting_params['min_val'] = combined_df['min_val'].unique().tolist()
            if 'max_val' in combined_df.columns:
                sorting_params['max_val'] = combined_df['max_val'].unique().tolist()

            self.config_params['sorting'] = sorting_params

        # Extract datastructures parameters
        if 'datastructures' in self.benchmark_types:
            ds_params = {}
            if 'data_structure' in combined_df.columns:
                ds_params['data_structures'] = combined_df['data_structure'].unique().tolist()
            if 'operation' in combined_df.columns:
                ds_params['operations'] = combined_df['operation'].unique().tolist()
            if 'size' in combined_df.columns:
                ds_params['sizes'] = sorted(combined_df['size'].unique().tolist())
            if 'run_index' in combined_df.columns:
                ds_params['runs_per_test'] = len(combined_df['run_index'].unique())
            if 'seed' in combined_df.columns:
                ds_params['seeds'] = combined_df['seed'].unique().tolist()
            if 'min_val' in combined_df.columns:
                ds_params['min_val'] = combined_df['min_val'].unique().tolist()
            if 'max_val' in combined_df.columns:
                ds_params['max_val'] = combined_df['max_val'].unique().tolist()

            self.config_params['datastructures'] = ds_params

    def get_combined_dataframe(self) -> pd.DataFrame:
        """Get combined dataframe from all sources."""
        if not self.dataframes:
            return pd.DataFrame()

        # Filter out empty dataframes to avoid pandas warnings
        non_empty_dfs = [df for df in self.dataframes if not df.empty]
        if not non_empty_dfs:
            return pd.DataFrame()

        combined = pd.concat(non_empty_dfs, ignore_index=True)

        # Remove duplicates based on key columns
        if self.benchmark_type == 'matmul':
            key_cols = ['size', 'algorithm', 'run_index', 'seed']
        elif self.benchmark_type == 'sorting':
            key_cols = ['size', 'algorithm', 'input_pattern', 'run_index', 'seed']
        elif self.benchmark_type == 'datastructures':
            key_cols = ['size', 'data_structure', 'operation', 'run_index', 'seed']
        else:
            return combined

        # Filter to existing columns
        existing_key_cols = [col for col in key_cols if col in combined.columns]
        if existing_key_cols:
            combined = combined.drop_duplicates(subset=existing_key_cols)

        return combined

    def get_dataframe_by_type(self, benchmark_type: str) -> pd.DataFrame:
        """Get dataframe filtered by benchmark type."""
        if not self.dataframes:
            return pd.DataFrame()

        # Filter out empty dataframes to avoid pandas warnings
        non_empty_dfs = [df for df in self.dataframes if not df.empty]
        if not non_empty_dfs:
            return pd.DataFrame()

        combined = pd.concat(non_empty_dfs, ignore_index=True)

        # Filter by benchmark type
        if benchmark_type == 'matmul':
            # Matmul: has algorithm, time_s, and either cutoff or block_size
            filtered = combined[
                (combined.get('algorithm').notna()) &
                (combined.get('time_s').notna()) &
                (
                    (combined.get('cutoff').notna()) |
                    (combined.get('block_size').notna())
                )
            ]
        elif benchmark_type == 'sorting':
            # Sorting: has algorithm, time_s, and input_pattern
            filtered = combined[
                (combined.get('algorithm').notna()) &
                (combined.get('time_s').notna()) &
                (combined.get('input_pattern').notna())
            ]
        elif benchmark_type == 'datastructures':
            # Datastructures: has data_structure, operation, and time_s
            filtered = combined[
                (combined.get('data_structure').notna()) &
                (combined.get('operation').notna()) &
                (combined.get('time_s').notna())
            ]
        else:
            return combined

        # Remove duplicates
        if benchmark_type == 'matmul':
            key_cols = ['size', 'algorithm', 'run_index', 'seed']
        elif benchmark_type == 'sorting':
            key_cols = ['size', 'algorithm', 'input_pattern', 'run_index', 'seed']
        elif benchmark_type == 'datastructures':
            key_cols = ['size', 'data_structure', 'operation', 'run_index', 'seed']
        else:
            return filtered

        existing_key_cols = [col for col in key_cols if col in filtered.columns]
        if existing_key_cols:
            filtered = filtered.drop_duplicates(subset=existing_key_cols)

        return filtered

    def get_statistics(self) -> pd.DataFrame:
        """Calculate statistics for each benchmark configuration."""
        df = self.get_combined_dataframe()
        if df.empty:
            return pd.DataFrame()

        if self.benchmark_type == 'matmul':
            group_cols = ['size', 'algorithm']
        elif self.benchmark_type == 'sorting':
            group_cols = ['size', 'algorithm', 'input_pattern']
        elif self.benchmark_type == 'datastructures':
            group_cols = ['size', 'data_structure', 'operation']
        else:
            return df

        # Filter to existing columns
        existing_group_cols = [col for col in group_cols if col in df.columns]
        if not existing_group_cols:
            return df

        stats = df.groupby(existing_group_cols)['time_s'].agg([
            'count', 'mean', 'median', 'std', 'min', 'max'
        ]).round(6).reset_index()

        return stats


class BenchmarkPlotter:
    """Unified plotter for different benchmark types."""

    def __init__(self, data: BenchmarkData):
        self.data = data
        self.df = data.get_combined_dataframe()
        self.stats = data.get_statistics()
        self.use_log_y = False  # Default to linear Y scale

    def plot_matmul_benchmark(self, output_file: str = None, show_theory: bool = True):
        """Plot matrix multiplication benchmark results."""
        df_matmul = self.data.get_dataframe_by_type('matmul')
        if df_matmul.empty:
            print("No matrix multiplication data to plot")
            return

        fig, ax = plt.subplots(figsize=(14, 10))

        # Get unique algorithms
        algorithms = sorted(df_matmul['algorithm'].unique())
        markers = ['o', 's', '^', 'D', 'v', 'p', '*', 'h', '8', 'P']

        # Build legend labels with algorithm-specific parameters
        legend_labels = self._build_matmul_legend_labels(algorithms, df_matmul)

        for i, algorithm in enumerate(algorithms):
            alg_data = df_matmul[df_matmul['algorithm'] == algorithm]

            # Group by size and calculate means
            size_means = alg_data.groupby('size')['time_s'].mean().reset_index()

            sizes = size_means['size'].values
            times = size_means['time_s'].values

            color = ColorScheme.get_algorithm_color(i)
            ax.plot(sizes, times, marker=markers[i % len(markers)],
                   color=color, linewidth=2, markersize=6,
                   label=legend_labels[i], alpha=0.8)

        # Add theoretical curves
        if show_theory and len(sizes) > 0:
            self._add_matmul_theory_curves(ax, sizes, algorithms)

        # Add configuration info
        self._add_config_info(ax, 'matmul', df_matmul)

        # Build clean title
        title = self._build_title_with_params('matmul')
        if self.use_log_y:
            title += " (Log Y Scale)"
        self._format_plot(ax, title, "Matrix Size (n)", "Time (seconds)")

        if self.use_log_y:
            ax.set_yscale('log')

        if output_file:
            plt.savefig(output_file, dpi=300, bbox_inches='tight')
            print(f"Matrix multiplication plot saved to: {output_file}")

        plt.show()

    def plot_sorting_benchmark(self, output_file: str = None, input_pattern: str = None):
        """Plot sorting benchmark results."""
        df_sorting = self.data.get_dataframe_by_type('sorting')
        if df_sorting.empty:
            print("No sorting data to plot")
            return

        # Filter by input pattern if specified
        plot_df = df_sorting
        if input_pattern:
            plot_df = df_sorting[df_sorting['input_pattern'] == input_pattern]

        if plot_df.empty:
            print(f"No sorting data for pattern: {input_pattern}")
            return

        fig, ax = plt.subplots(figsize=(14, 10))

        # Get unique combinations
        algorithms = sorted(plot_df['algorithm'].unique())
        patterns = sorted(plot_df['input_pattern'].unique()) if 'input_pattern' in plot_df.columns else ['default']

        markers = ['o', 's', '^', 'D', 'v', 'p', '*', 'h', '8', 'P']

        # Create mapping of algorithms to colors (consistent across patterns)
        algorithm_colors = {}
        for idx, algorithm in enumerate(algorithms):
            algorithm_colors[algorithm] = ColorScheme.get_algorithm_color(idx)

        plot_idx = 0
        for i, algorithm in enumerate(algorithms):
            for j, pattern in enumerate(patterns):
                alg_pattern_data = plot_df[
                    (plot_df['algorithm'] == algorithm) &
                    (plot_df['input_pattern'] == pattern)
                ]

                if alg_pattern_data.empty:
                    continue

                size_means = alg_pattern_data.groupby('size')['time_s'].mean().reset_index()

                sizes = size_means['size'].values
                times = size_means['time_s'].values

                # Enhanced label with pattern information
                if len(patterns) > 1:
                    label = f"{algorithm} • {pattern}"
                else:
                    label = algorithm

                color = algorithm_colors[algorithm]  # Same color for same algorithm
                ax.plot(sizes, times, marker=markers[j % len(markers)],
                       color=color, linewidth=2, markersize=6,
                       label=label, alpha=0.8)
                plot_idx += 1

        # Add configuration info
        self._add_config_info(ax, 'sorting', plot_df)

        # Build comprehensive title with parameters
        title = self._build_title_with_params('sorting', input_pattern)
        if self.use_log_y:
            title += " (Log Y Scale)"
        self._format_plot(ax, title, "Array Size (n)", "Time (seconds)")

        plt.yscale('log')

        if output_file:
            plt.savefig(output_file, dpi=300, bbox_inches='tight')
            print(f"Sorting plot saved to: {output_file}")

        plt.show()

    def plot_datastructures_benchmark(self, output_file: str = None):
        """Plot data structures benchmark results."""
        df_ds = self.data.get_dataframe_by_type('datastructures')
        if df_ds.empty:
            print("No data structures data to plot")
            return

        # Remove the largest size (outlier) for better visualization
        max_size = df_ds['size'].max()
        df_ds_filtered = df_ds[df_ds['size'] < max_size]
        print(f"Filtered out largest size {max_size:,} from datastructures plot")

        # Cut X axis to ensure all curves have the same number of points
        common_sizes = None
        if not df_ds_filtered.empty:
            grouped = df_ds_filtered.groupby(['data_structure', 'operation'])
            for _, g in grouped:
                sizes_set = set(g['size'].unique().tolist())
                if common_sizes is None:
                    common_sizes = sizes_set
                else:
                    common_sizes &= sizes_set

        if common_sizes:
            common_sizes_list = sorted(common_sizes)
            df_ds_filtered = df_ds_filtered[df_ds_filtered['size'].isin(common_sizes_list)]
            print(f"Aligned all curves to common sizes ({len(common_sizes_list)} points): {', '.join(map(lambda v: f'{int(v):,}', common_sizes_list))}")
        else:
            print("Warning: Could not find common sizes across curves; using available sizes per curve.")

        fig, ax = plt.subplots(figsize=(14, 10))

        # Get unique combinations
        data_structures = sorted(df_ds_filtered['data_structure'].unique())
        operations = sorted(df_ds_filtered['operation'].unique())

        markers = ['o', 's', '^', 'D', 'v', 'p', '*', 'h', '8', 'P']

        # Create mapping of data structures to colors (consistent across operations)
        structure_colors = {}
        for idx, ds in enumerate(data_structures):
            structure_colors[ds] = ColorScheme.get_structure_color(idx)

        for i, ds in enumerate(data_structures):
            for j, op in enumerate(operations):
                ds_op_data = df_ds_filtered[
                    (df_ds_filtered['data_structure'] == ds) &
                    (df_ds_filtered['operation'] == op)
                ]

                if ds_op_data.empty:
                    continue

                size_means = ds_op_data.groupby('size')['time_s'].mean().reset_index()

                sizes = size_means['size'].values
                times = size_means['time_s'].values

                # Enhanced label with operation information
                label = f"{ds} • {op}"
                color = structure_colors[ds]  # Same color for same data structure
                ax.plot(sizes, times, marker=markers[j % len(markers)],
                       color=color, linewidth=2, markersize=6,
                       label=label, alpha=0.8)

        # Add configuration info
        self._add_config_info(ax, 'datastructures', df_ds_filtered)

        # Build comprehensive title with parameters
        title = self._build_title_with_params('datastructures')
        if self.use_log_y:
            title += " (Log Y Scale)"
        self._format_plot(ax, title, "Size (n)", "Time (seconds)")

        # Set explicit X ticks based on unique sizes (common across curves)
        all_sizes = sorted(df_ds_filtered['size'].unique())
        ax.set_xticks(all_sizes)

        # Custom labels: show first value, blanks for values <= 100000 (except first), '...' for values < 800000, and all >= 800000
        labels = []
        threshold = 800000
        blank_until = 100000
        for idx, size in enumerate(all_sizes):
            if idx == 0:
                labels.append(f"{int(size):,}")
            elif size <= blank_until:
                labels.append("")
            elif size < threshold:
                labels.append("...")
            else:
                labels.append(f"{int(size):,}")
        ax.set_xticklabels(labels, rotation=45, ha='right')

        plt.yscale('log')

        if self.use_log_y:
            ax.set_yscale('log')

        if output_file:
            plt.savefig(output_file, dpi=300, bbox_inches='tight')
            print(f"Data structures plot saved to: {output_file}")

        plt.show()

    def _add_matmul_theory_curves(self, ax, sizes, algorithms):
        """Add theoretical complexity curves for matrix multiplication."""
        if len(sizes) == 0:
            return

        min_size = min(sizes)
        max_size = max(sizes)

        # Get reference time from the mean of all smallest sizes across all algorithms
        # This provides a more representative baseline for theoretical curves

        # Find the smallest size for each algorithm
        algorithm_min_times = []
        for algorithm in algorithms:
            alg_data = self.df[self.df['algorithm'] == algorithm]
            if not alg_data.empty:
                # Get the smallest size for this algorithm
                alg_min_size = alg_data['size'].min()
                # Get data for this smallest size
                alg_min_data = alg_data[alg_data['size'] == alg_min_size]
                if not alg_min_data.empty:
                    # Add the mean time for this algorithm's smallest size
                    algorithm_min_times.append(alg_min_data['time_s'].mean())

        if not algorithm_min_times:
            # Fallback to original method if no algorithm-specific data
            ref_data = self.df[self.df['size'] == min_size]
            if ref_data.empty:
                return
            ref_time = ref_data['time_s'].mean()
        else:
            # Use mean of all algorithm minimum times for more stable reference
            ref_time = np.mean(algorithm_min_times)

        # Generate theory curve points
        theory_sizes = np.logspace(np.log10(min_size), np.log10(max_size), 100)

        # O(n³) curve
        o_n3 = ref_time * (theory_sizes / min_size) ** 3.0
        ax.plot(theory_sizes, o_n3, '--', color='red', alpha=0.7,
               label='O(n³)', linewidth=2)

        # O(n^2.81) curve (Strassen)
        o_n281 = ref_time * (theory_sizes / min_size) ** 2.807
        ax.plot(theory_sizes, o_n281, '--', color='blue', alpha=0.7,
               label='O(n^2.81)', linewidth=2)

    def _add_config_info(self, ax, benchmark_type: str, plot_df: pd.DataFrame = None):
        """Add configuration information to the plot.

        If plot_df is provided, configuration values (algorithms, sizes, params)
        are derived from the specific data being plotted. Otherwise falls back
        to aggregated config_params.
        """
        info_lines = []

        # Helper to clean numeric lists and return min-max string
        def format_range(values):
            if values is None:
                return None
            clean_vals = [int(v) for v in values if v is not None and not pd.isna(v)]
            if not clean_vals:
                return None
            vmin, vmax = min(clean_vals), max(clean_vals)
            if vmin == vmax:
                return f"{vmin:,}"
            return f"{vmin:,}–{vmax:,}"

        # Prefer deriving from plot_df when available
        if plot_df is not None and not plot_df.empty:
            if benchmark_type == 'matmul':
                algos = sorted(plot_df['algorithm'].dropna().unique().tolist()) if 'algorithm' in plot_df.columns else []
                if algos:
                    info_lines.append(f"Algorithms: {', '.join(map(str, algos))}")
                sizes = plot_df['size'].dropna().unique().tolist() if 'size' in plot_df.columns else []
                sizes_str = format_range(sizes)
                if sizes_str:
                    info_lines.append(f"Sizes: {sizes_str}")
                # Derive runs from run_index if present
                if 'run_index' in plot_df.columns:
                    runs = len(pd.Series(plot_df['run_index'].dropna().unique()))
                    if runs:
                        info_lines.append(f"Runs per test: {runs}")
                # Algorithm-specific parameters
                for algo in algos:
                    algo_mask = (plot_df['algorithm'] == algo) if 'algorithm' in plot_df.columns else None
                    algo_l = str(algo).lower()
                    # Strassen uses cutoff
                    if 'strassen' in algo_l and 'cutoff' in plot_df.columns:
                        vals = plot_df.loc[algo_mask, 'cutoff'].dropna().unique().tolist()
                        cutoff_str = format_range(vals)
                        if cutoff_str:
                            info_lines.append(f"{algo} cutoff: {cutoff_str}")
                    # Blocked variants use block_size
                    if ('block' in algo_l) and 'block_size' in plot_df.columns:
                        vals = plot_df.loc[algo_mask, 'block_size'].dropna().unique().tolist()
                        block_str = format_range(vals)
                        if block_str:
                            info_lines.append(f"{algo} block size: {block_str}")

            elif benchmark_type == 'sorting':
                algos = sorted(plot_df['algorithm'].dropna().unique().tolist()) if 'algorithm' in plot_df.columns else []
                if algos:
                    info_lines.append(f"Algorithms: {', '.join(map(str, algos))}")
                pats = sorted(plot_df['input_pattern'].dropna().unique().tolist()) if 'input_pattern' in plot_df.columns else []
                if pats:
                    info_lines.append(f"Patterns: {', '.join(map(str, pats))}")
                sizes = plot_df['size'].dropna().unique().tolist() if 'size' in plot_df.columns else []
                sizes_str = format_range(sizes)
                if sizes_str:
                    info_lines.append(f"Sizes: {sizes_str}")
                if 'run_index' in plot_df.columns:
                    runs = len(pd.Series(plot_df['run_index'].dropna().unique()))
                    if runs:
                        info_lines.append(f"Runs per test: {runs}")

            elif benchmark_type == 'datastructures':
                dss = sorted(plot_df['data_structure'].dropna().unique().tolist()) if 'data_structure' in plot_df.columns else []
                if dss:
                    info_lines.append(f"Structures: {', '.join(map(str, dss))}")
                ops = sorted(plot_df['operation'].dropna().unique().tolist()) if 'operation' in plot_df.columns else []
                if ops:
                    info_lines.append(f"Operations: {', '.join(map(str, ops))}")
                sizes = plot_df['size'].dropna().unique().tolist() if 'size' in plot_df.columns else []
                sizes_str = format_range(sizes)
                if sizes_str:
                    info_lines.append(f"Sizes: {sizes_str}")
                if 'run_index' in plot_df.columns:
                    runs = len(pd.Series(plot_df['run_index'].dropna().unique()))
                    if runs:
                        info_lines.append(f"Runs per test: {runs}")
        else:
            # Fallback to global config when plot_df is not provided
            if benchmark_type not in self.data.config_params:
                return
            config = self.data.config_params[benchmark_type]

            if benchmark_type == 'matmul':
                if 'algorithms' in config:
                    algos = [str(a) for a in config.get('algorithms', []) if a is not None and not pd.isna(a)]
                    if algos:
                        info_lines.append(f"Algorithms: {', '.join(algos)}")
                if 'sizes' in config:
                    sizes_str = format_range(config.get('sizes'))
                    if sizes_str:
                        info_lines.append(f"Sizes: {sizes_str}")
                if 'runs_per_test' in config and config['runs_per_test']:
                    info_lines.append(f"Runs per test: {config['runs_per_test']}")
                if 'cutoff' in config:
                    cutoff_str = format_range(config.get('cutoff'))
                    if cutoff_str:
                        info_lines.append(f"Cutoff: {cutoff_str}")
                if 'block_size' in config:
                    block_str = format_range(config.get('block_size'))
                    if block_str:
                        info_lines.append(f"Block size: {block_str}")

            elif benchmark_type == 'sorting':
                if 'algorithms' in config:
                    algos = [str(a) for a in config.get('algorithms', []) if a is not None and not pd.isna(a)]
                    if algos:
                        info_lines.append(f"Algorithms: {', '.join(algos)}")
                if 'input_patterns' in config:
                    pats = [str(p) for p in config.get('input_patterns', []) if p is not None and not pd.isna(p)]
                    if pats:
                        info_lines.append(f"Patterns: {', '.join(pats)}")
                if 'sizes' in config:
                    sizes_str = format_range(config.get('sizes'))
                    if sizes_str:
                        info_lines.append(f"Sizes: {sizes_str}")
                if 'runs_per_test' in config and config['runs_per_test']:
                    info_lines.append(f"Runs per test: {config['runs_per_test']}")

            elif benchmark_type == 'datastructures':
                if 'data_structures' in config:
                    dss = [str(ds) for ds in config.get('data_structures', []) if ds is not None and not pd.isna(ds)]
                    if dss:
                        info_lines.append(f"Structures: {', '.join(dss)}")
                if 'operations' in config:
                    ops = [str(op) for op in config.get('operations', []) if op is not None and not pd.isna(op)]
                    if ops:
                        info_lines.append(f"Operations: {', '.join(ops)}")
                if 'sizes' in config:
                    sizes_str = format_range(config.get('sizes'))
                    if sizes_str:
                        info_lines.append(f"Sizes: {sizes_str}")
                if 'runs_per_test' in config and config['runs_per_test']:
                    info_lines.append(f"Runs per test: {config['runs_per_test']}")

        # Add metadata
        if self.data.metadata:
            if 'cpu_model' in self.data.metadata and self.data.metadata['cpu_model']:
                info_lines.append(f"CPU: {self.data.metadata['cpu_model']}")
            if 'hostname' in self.data.metadata and self.data.metadata['hostname']:
                info_lines.append(f"Host: {self.data.metadata['hostname']}")
            if 'timestamp' in self.data.metadata and self.data.metadata['timestamp']:
                info_lines.append(f"Timestamp: {self.data.metadata['timestamp']}")

        # Add info box to plot
        if info_lines:
            info_text = "\n".join(info_lines)
            ax.text(0.02, 0.98, info_text, transform=ax.transAxes,
                   fontsize=9, verticalalignment='top',
                   bbox=dict(boxstyle='round,pad=0.5', facecolor='white', alpha=0.8))

    def _build_title_with_params(self, benchmark_type: str, input_pattern: str = None) -> str:
        """Build clean title with only essential parameters."""
        if benchmark_type not in self.data.config_params:
            # Fallback titles
            if benchmark_type == 'matmul':
                return "Matrix Multiplication Benchmark"
            elif benchmark_type == 'sorting':
                base_title = "Sorting Algorithms Benchmark"
                if input_pattern:
                    base_title += f" - {input_pattern}"
                return base_title
            elif benchmark_type == 'datastructures':
                return "Data Structures Benchmark"
            return f"{benchmark_type.title()} Benchmark"

        config = self.data.config_params[benchmark_type]
        title_parts = []

        if benchmark_type == 'matmul':
            title_parts.append("Matrix Multiplication")

            # Add runs per test only (other params go to legend)
            if 'runs_per_test' in config:
                title_parts.append(f"{config['runs_per_test']} runs")

        elif benchmark_type == 'sorting':
            title_parts.append("Sorting Algorithms")

            # Add input patterns if specific one is selected
            if input_pattern:
                title_parts.append(f"{input_pattern}")

            # Add runs per test only
            if 'runs_per_test' in config:
                title_parts.append(f"{config['runs_per_test']} runs")

        elif benchmark_type == 'datastructures':
            title_parts.append("Data Structures")

            # Add runs per test only
            if 'runs_per_test' in config:
                title_parts.append(f"{config['runs_per_test']} runs")

        # Join all parts with appropriate separators
        if len(title_parts) > 1:
            title = title_parts[0] + " Benchmark"
            params = " • ".join(title_parts[1:])
            title += f" • {params}"
        else:
            title = title_parts[0] + " Benchmark" if title_parts else f"{benchmark_type.title()} Benchmark"

        return title

    def _build_matmul_legend_labels(self, algorithms, df_matmul):
        """Build legend labels with algorithm-specific parameters for matmul."""
        legend_labels = []

        for algorithm in algorithms:
            alg_data = df_matmul[df_matmul['algorithm'] == algorithm]

            # Start with algorithm name
            label_parts = [algorithm]

            # Add cutoff if present and unique for this algorithm
            if 'cutoff' in alg_data.columns:
                cutoff_values = alg_data['cutoff'].unique()
                if len(cutoff_values) == 1 and not pd.isna(cutoff_values[0]):
                    label_parts.append(f"cutoff={int(cutoff_values[0])}")

            # Add block_size if present and unique for this algorithm
            if 'block_size' in alg_data.columns:
                block_values = alg_data['block_size'].unique()
                if len(block_values) == 1 and not pd.isna(block_values[0]):
                    label_parts.append(f"block={int(block_values[0])}")

            # Join parts
            legend_labels.append(" • ".join(label_parts))

        return legend_labels

    def plot_all_benchmarks(self, output_dir: str = None, show_theory: bool = True):
        """Plot all detected benchmark types."""
        if not self.data.benchmark_types:
            print("No benchmark types detected")
            return

        print(f"Detected benchmark types: {', '.join(self.data.benchmark_types)}")

        for benchmark_type in ['matmul', 'sorting', 'datastructures']:
            if benchmark_type in self.data.benchmark_types:
                print(f"\nGenerating {benchmark_type} plot...")

                output_file = None
                if output_dir:
                    os.makedirs(output_dir, exist_ok=True)
                    output_file = os.path.join(output_dir, f'{benchmark_type}_benchmark.png')

                if benchmark_type == 'matmul':
                    self.plot_matmul_benchmark(output_file, show_theory)
                elif benchmark_type == 'sorting':
                    self.plot_sorting_benchmark(output_file)
                elif benchmark_type == 'datastructures':
                    self.plot_datastructures_benchmark(output_file)

        print(f"\nAll plots generated successfully!")
        if output_dir:
            print(f"Plots saved to: {output_dir}")

    def _format_plot(self, ax, title: str, xlabel: str, ylabel: str):
        """Format plot with common settings."""
        ax.set_title(title, fontsize=16, fontweight='bold')
        ax.set_xlabel(xlabel, fontsize=14)
        ax.set_ylabel(ylabel, fontsize=14)
        ax.legend(fontsize=12, loc='upper right', bbox_to_anchor=(0.98, 0.98))
        ax.grid(True, alpha=0.3)
        ax.tick_params(axis='both', which='major', labelsize=12)

        # Format ticks for log scale
        if ax.get_xscale() == 'log':
            ax.xaxis.set_major_formatter(plt.ScalarFormatter())
        else:
            # For linear X scale, ensure integer formatting for size values
            ax.xaxis.set_major_formatter(plt.FuncFormatter(lambda x, p: f'{int(x):,}'))
        if ax.get_yscale() == 'log':
            ax.yaxis.set_major_formatter(plt.ScalarFormatter())

    def print_summary(self):
        """Print summary of benchmark results."""
        if self.stats.empty:
            print("No data available for summary")
            return

        print("\n" + "="*80)
        print("BENCHMARK SUMMARY")
        print("="*80)

        if self.data.benchmark_type:
            print(f"Benchmark Type: {self.data.benchmark_type.upper()}")

        if self.data.metadata:
            print("Environment:")
            for key, value in self.data.metadata.items():
                print(f"  {key}: {value}")

        print(f"\nTotal Measurements: {len(self.df)}")

        if self.data.benchmark_type == 'matmul':
            print("\nMatrix Multiplication Results:")
            print("-" * 50)
            for _, row in self.stats.iterrows():
                print(f"Size {int(row['size']):4d} | {row['algorithm']:12s} | "
                      f"Mean: {row['mean']:.6f}s | Median: {row['median']:.6f}s")

        elif self.data.benchmark_type == 'sorting':
            print("\nSorting Results:")
            print("-" * 70)
            for _, row in self.stats.iterrows():
                pattern = row.get('input_pattern', 'N/A')
                print(f"Size {int(row['size']):5d} | {row['algorithm']:12s} | {pattern:12s} | "
                      f"Mean: {row['mean']:.6f}s")

        elif self.data.benchmark_type == 'datastructures':
            print("\nData Structures Results:")
            print("-" * 60)
            for _, row in self.stats.iterrows():
                print(f"Size {int(row['size']):5d} | {row['data_structure']:10s} | {row['operation']:12s} | "
                      f"Mean: {row['mean']:.6f}s")

        print("="*80)


def main():
    """Main function."""
    parser = argparse.ArgumentParser(
        description='Unified benchmark plotting script',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  %(prog)s results.csv                                # Plot single CSV file (auto-detect type)
  %(prog)s *.csv                                       # Plot all CSV files (all types detected)
  %(prog)s --csv bench1.csv --json bench2.json        # Combine multiple files
  %(prog)s results/*.csv --output-dir plots/          # Generate all plots to directory
  %(prog)s sorting_results.csv --type sorting         # Force specific type
  %(prog)s --csv bench.csv --log-y                    # Use logarithmic Y scale
  %(prog)s --csv bench.csv --no-theory               # Disable theoretical curves
  %(prog)s results/*.csv --summary-only              # Print summary only
        """
    )

    parser.add_argument('files', nargs='*',
                       help='Input files (CSV or JSON, auto-detected)')

    parser.add_argument('--csv', action='append',
                       help='CSV input files')

    parser.add_argument('--json', action='append',
                       help='JSON input files')

    parser.add_argument('--output', '-o', type=str, default=None,
                       help='Output plot file (PNG format) - single type only')

    parser.add_argument('--output-dir', type=str, default=None,
                       help='Output directory for all plots (PNG format)')

    parser.add_argument('--type', choices=['matmul', 'sorting', 'datastructures'],
                       help='Force benchmark type (auto-detected by default)')

    parser.add_argument('--input-pattern', type=str,
                       help='Filter sorting by input pattern (ordered, reverse, random, etc.)')

    parser.add_argument('--log-y', action='store_true',
                       help='Use logarithmic scale for Y axis')

    parser.add_argument('--no-theory', action='store_true',
                       help='Disable theoretical complexity curves')

    parser.add_argument('--summary-only', action='store_true',
                       help='Print summary only, no plotting')

    args = parser.parse_args()

    # Collect all input files
    csv_files = args.csv or []
    json_files = args.json or []

    # Auto-detect from positional arguments
    for file in args.files:
        if file.endswith('.csv'):
            csv_files.append(file)
        elif file.endswith('.json'):
            json_files.append(file)
        else:
            # Try CSV first, then JSON
            if os.path.exists(file + '.csv'):
                csv_files.append(file + '.csv')
            elif os.path.exists(file + '.json'):
                json_files.append(file + '.json')
            else:
                print(f"Warning: File not found: {file}")

    if not csv_files and not json_files:
        print("Error: No input files specified")
        parser.print_help()
        sys.exit(1)

    # Load data
    data = BenchmarkData(csv_files, json_files)

    if not data.benchmark_types:
        print("Error: No benchmark types detected in the input files")
        sys.exit(1)

    print(f"Detected benchmark types: {', '.join(data.benchmark_types)}")

    # Override type if specified
    if args.type:
        data.benchmark_types = {args.type}
        data.benchmark_type = args.type

    # Create plotter
    plotter = BenchmarkPlotter(data)
    plotter.use_log_y = args.log_y  # Set log scale preference

    # Print summary
    plotter.print_summary()

    if args.summary_only:
        return

    # Decide whether to plot all benchmarks or a specific one
    if len(data.benchmark_types) > 1 or args.output_dir:
        # Multiple types detected or output directory specified - plot all
        plotter.plot_all_benchmarks(args.output_dir, not args.no_theory)
    else:
        # Single type - use specific plotting method
        output_file = args.output or args.output_dir
        if output_file and args.output_dir:
            # If output_dir is specified, use it as directory
            output_file = os.path.join(args.output_dir, f"{data.benchmark_type}_benchmark.png")

        if data.benchmark_type == 'matmul':
            plotter.plot_matmul_benchmark(output_file, not args.no_theory)
        elif data.benchmark_type == 'sorting':
            plotter.plot_sorting_benchmark(output_file, args.input_pattern)
        elif data.benchmark_type == 'datastructures':
            plotter.plot_datastructures_benchmark(output_file)
        else:
            print("Error: Unsupported benchmark type for plotting")
            sys.exit(1)


if __name__ == '__main__':
    main()
