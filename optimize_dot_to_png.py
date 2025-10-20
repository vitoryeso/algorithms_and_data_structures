#!/usr/bin/env python3
"""
Script otimizado para converter grafo DOT de MLP grande para PNG
Usa NetworkX e Matplotlib para renderização eficiente sem labels
"""

import networkx as nx
import matplotlib.pyplot as plt
import numpy as np
from collections import defaultdict
import sys

def load_dot_graph(dot_file):
    """Carrega grafo DOT de forma eficiente"""
    print(f"🔄 Iniciando carregamento do grafo: {dot_file}")
    print("   Arquivo muito grande (>325k linhas), isso pode levar alguns minutos...")

    try:
        print("   📚 Carregando arquivo DOT com pydot...")
        # Usa pydot para ler DOT (mais eficiente que graphviz)
        import pydot
        graphs = pydot.graph_from_dot_file(dot_file)
        print("   ✅ Arquivo DOT lido com sucesso")

        if graphs:
            print("   🔄 Convertendo para NetworkX...")
            # Converte pydot para networkx
            G = nx.nx_pydot.from_pydot(graphs[0])
            print(f"   ✅ Grafo convertido: {G.number_of_nodes()} nós, {G.number_of_edges()} arestas")
            return G
        else:
            print("❌ Erro: Não foi possível carregar o grafo")
            return None
    except ImportError:
        print("   ⚠️  pydot não disponível, tentando com graphviz...")
        try:
            G = nx.nx_agraph.read_dot(dot_file)
            print(f"   ✅ Grafo carregado: {G.number_of_nodes()} nós, {G.number_of_edges()} arestas")
            return G
        except ImportError:
            print("❌ Erro: Nem pydot nem pygraphviz disponíveis")
            print("   Instale com: pip install pydot")
            return None
    except Exception as e:
        print(f"❌ Erro ao carregar grafo: {e}")
        return None

def assign_node_positions(G):
    """Atribui posições aos nós baseado nas camadas MLP"""
    print("🔄 Atribuindo posições aos nós...")
    print(f"   Total de nós a processar: {G.number_of_nodes()}")

    # Agrupa nós por camada
    layers = defaultdict(list)
    layer_colors = {}

    # Cores por camada
    colors = ['#FF6B6B', '#4ECDC4', '#45B7D1', '#96CEB4', '#FFEAA7',
              '#DDA0DD', '#98D8C8', '#F7DC6F', '#BB8FCE', '#85C1E9']

    print("   📊 Agrupando nós por camada...")
    for node in G.nodes():
        node_str = str(node)
        if node_str.startswith('input_'):
            layers[0].append(node)
            layer_colors[0] = colors[0]
        elif node_str.startswith('hidden1_'):
            layers[1].append(node)
            layer_colors[1] = colors[1]
        elif node_str.startswith('hidden2_'):
            layers[2].append(node)
            layer_colors[2] = colors[2]
        elif node_str.startswith('hidden3_'):
            layers[3].append(node)
            layer_colors[3] = colors[3]
        elif node_str.startswith('output_'):
            # Agrupa outputs em uma camada
            layers[4].append(node)
            layer_colors[4] = colors[4]

    # Mostra estatísticas das camadas
    print("   📈 Estatísticas das camadas:")
    for layer_idx in sorted(layers.keys()):
        print(f"      Camada {layer_idx}: {len(layers[layer_idx])} nós")

    # Atribui posições
    print("   📍 Calculando posições espaciais...")
    pos = {}
    layer_spacing = 2.0
    max_nodes_per_layer = max(len(nodes) for nodes in layers.values())
    print(f"   Máximo de nós por camada: {max_nodes_per_layer}")

    for layer_idx, nodes in layers.items():
        x = layer_idx * layer_spacing
        y_spacing = 1.0 / max(1, len(nodes) - 1) if len(nodes) > 1 else 0.5

        for i, node in enumerate(sorted(nodes, key=lambda x: int(str(x).split('_')[-1]))):
            y = (i * y_spacing) - 0.5  # Centraliza na vertical
            pos[node] = (x, y)

    print(f"   ✅ Posições atribuídas para {len(pos)} nós")
    return pos, layer_colors

def draw_optimized_graph(G, pos, layer_colors, output_file, dpi=300):
    """Desenha grafo de forma otimizada para grandes MLPs"""
    print(f"🎨 Iniciando renderização do grafo com DPI={dpi}...")
    print(f"   Arquivo de saída: {output_file}")
    print(f"   Total de arestas: {G.number_of_edges()} (pode demorar um pouco)")

    print("   📊 Criando figura matplotlib...")
    plt.figure(figsize=(20, 12))

    print("   🖊️  Desenhando arestas...")
    # Desenha arestas primeiro (mais finas para performance)
    nx.draw_networkx_edges(G, pos,
                          edge_color='#CCCCCC',
                          alpha=0.3,
                          width=0.1,
                          arrows=False)
    print("   ✅ Arestas desenhadas")

    # Desenha nós por camada com cores
    print("   🎯 Desenhando nós por camada...")
    for layer_idx, color in layer_colors.items():
        layer_nodes = [node for node in G.nodes()
                      if (str(node).startswith('input_') and layer_idx == 0) or
                         (str(node).startswith('hidden1_') and layer_idx == 1) or
                         (str(node).startswith('hidden2_') and layer_idx == 2) or
                         (str(node).startswith('hidden3_') and layer_idx == 3) or
                         (str(node).startswith('output_') and layer_idx == 4)]

        if layer_nodes:
            node_sizes = [10] * len(layer_nodes)  # Nós pequenos para performance
            print(f"      Camada {layer_idx}: {len(layer_nodes)} nós...")
            nx.draw_networkx_nodes(G, pos,
                                 nodelist=layer_nodes,
                                 node_color=color,
                                 node_size=node_sizes,
                                 alpha=0.8)

    print("   🎨 Finalizando layout...")
    # Remove eixos e labels
    plt.axis('off')
    plt.tight_layout()

    print("   💾 Salvando arquivo PNG (isso pode demorar)...")
    # Salva com alta resolução
    plt.savefig(output_file, dpi=dpi, bbox_inches='tight', pad_inches=0.1)
    plt.close()

    print(f"   ✅ Grafo salvo em: {output_file}")
    print("   🎉 Renderização concluída!")

def main():
    print("🚀 Iniciando conversão otimizada DOT → PNG")
    print("=" * 50)

    if len(sys.argv) != 3:
        print("❌ Uso: python optimize_dot_to_png.py <arquivo_dot> <arquivo_png>")
        sys.exit(1)

    dot_file = sys.argv[1]
    png_file = sys.argv[2]

    print(f"📁 Arquivo de entrada: {dot_file}")
    print(f"📤 Arquivo de saída: {png_file}")
    print()

    # Carrega grafo
    print("1️⃣ CARREGANDO GRAFO")
    print("-" * 30)
    G = load_dot_graph(dot_file)
    if G is None:
        print("❌ Falha no carregamento do grafo!")
        sys.exit(1)
    print()

    # Atribui posições
    print("2️⃣ ATRIBUINDO POSIÇÕES")
    print("-" * 30)
    pos, layer_colors = assign_node_positions(G)
    print()

    # Renderiza e salva
    print("3️⃣ RENDERIZANDO E SALVANDO")
    print("-" * 30)
    draw_optimized_graph(G, pos, layer_colors, png_file)

    print()
    print("🎉 CONVERSÃO CONCLUÍDA COM SUCESSO!")
    print("=" * 50)

if __name__ == "__main__":
    main()
