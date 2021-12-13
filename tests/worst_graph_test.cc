#include <gtest/gtest.h>
#include <limits.h>
#include "../data_structures/c/worst_graph.h"


TEST(WorstGraphTest, NodeInserting) {
  struct graph* G; 
  G = inicializar(10, true);

  std::string nodes[8] = {"natal", "parnamirim", "macaiba", "mossoro", "currais novos", "acari", "martins", "acu" };
  char* char_nodes[8];

  for (char i=0; i<8; i++) {
    char* name;
    name = const_cast<char*>(nodes[i].c_str());
    inserirVertice(G, name);
  }

  print_graph(G);

  for (char i=0; i<8; i++) {
    char* name;
    name = const_cast<char*>(nodes[i].c_str());
    EXPECT_EQ(i, obterIndiceVertice(G, name));
  }
  EXPECT_EQ(8, G->nodes->len);
}

TEST(WorstGraphTest, EdgeInserting) {
  struct graph* G; 
  G = inicializar(10, true);

  std::string nodes[3] = {"natal", "parnamirim", "macaiba"};
  char* char_nodes[3];

  for (char i=0; i<3; i++) {
    char* name;
    name = const_cast<char*>(nodes[i].c_str());
    inserirVertice(G, name);
  }

  inserirAresta(G, const_cast<char*>(nodes[0].c_str()), const_cast<char*>(nodes[1].c_str()), INT_MAX);
  inserirAresta(G, const_cast<char*>(nodes[0].c_str()), const_cast<char*>(nodes[2].c_str()), INT_MAX);
  print_graph(G);
  EXPECT_EQ(saoConectados(G, 
        const_cast<char*>(nodes[0].c_str()),
        const_cast<char*>(nodes[1].c_str())), true);
  EXPECT_EQ(saoConectados(G, 
        const_cast<char*>(nodes[1].c_str()),
        const_cast<char*>(nodes[2].c_str())), false);
}
