#include <gtest/gtest.h>
#include "../data_structures/c/str_array.h"
#include "../data_structures/c/edge_array.h"
#include <string>

// Demonstrate some basic assertions.
TEST(C_ARRAYS_TESTS, STR_ARR) {
  struct str_arr* lu_names;
  lu_names = (struct str_arr*) malloc(sizeof(void*));
  std::string str_name("y3sooooo");
  char* name = const_cast<char*>(str_name.c_str());

  STRARRallocate(lu_names, 5);
  STRARRappend(lu_names, name);
  STRARRappend(lu_names, name);
  STRARRappend(lu_names, name);

  EXPECT_EQ(name, lu_names->names[0]);
  EXPECT_EQ(5, lu_names->cap);

  STRARRappend(lu_names, name);
  STRARRappend(lu_names, name);
  STRARRappend(lu_names, name);
  STRARRappend(lu_names, name);
  STRARRappend(lu_names, name);

  EXPECT_EQ(10, lu_names->cap);
  EXPECT_EQ(8, lu_names->len);
}

TEST(C_ARRAYS_TESTS, EDGE_ARR) {
  struct edge_arr* lu_edges;
  struct edge* lu_edge;
  lu_edge = (struct edge*) malloc(sizeof(void*));

  lu_edge->dest_id = 0;
  lu_edge->orig_id = 3;
  lu_edge->weight = 10;

  lu_edges = (struct edge_arr*) malloc(sizeof(void*));

  EDGARRallocate(lu_edges, 5);
  EXPECT_EQ(lu_edges->cap, 5);
  EDGARRappend(lu_edges, *lu_edge);
  EXPECT_EQ(lu_edges->len, 1);
  EXPECT_EQ(lu_edges->edges[0].dest_id, lu_edge->dest_id);
  EXPECT_EQ(lu_edges->edges[0].orig_id, lu_edge->orig_id);
  EXPECT_EQ(lu_edges->edges[0].weight, lu_edge->weight);
}
