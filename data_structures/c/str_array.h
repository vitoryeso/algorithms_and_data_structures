#ifndef _STR_ARRAY_
#define _STR_ARRAY_

#include <stdbool.h>

struct str_arr {
  char** names;
  unsigned len;
  unsigned cap;
};

void STRARRfree_all(struct str_arr* arr);
void STRARRcopy(struct str_arr* arr, struct str_arr* arr2);
void STRARRallocate(struct str_arr* arr, unsigned cap);
void STRARRduplicate_mem(struct str_arr* arr);

void STRARRprint_arr(struct str_arr* arr);
void STRARRappend(struct str_arr* arr, char* name);
void STRARRremove_end(struct str_arr* arr);
bool STRARRremove_elem(struct str_arr* arr, unsigned i);
bool STRARRinsert_elem(struct str_arr* arr, char* name, unsigned i);

#endif
