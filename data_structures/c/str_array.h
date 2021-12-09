#ifndef _STR_ARRAY_
#define _STR_ARRAY_

#include <stdbool.h>

struct str_arr {
  char** names;
  unsigned len;
  unsigned cap;
};

void free_all(struct str_arr* arr);
void copy(struct str_arr* arr, struct str_arr* arr2);
void allocate(struct str_arr* arr, unsigned cap);
void duplicate_mem(struct str_arr* arr);

void printarr(struct str_arr* arr);
void append(struct str_arr* arr, char* name);
void remove_end(struct str_arr* arr);
bool remove_elem(struct str_arr* arr, unsigned i);
bool insert_elem(struct str_arr* arr, char* name, unsigned i);

#endif
