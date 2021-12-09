#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "./str_array.h"

void allocate(struct str_arr* arr, unsigned cap) {
  arr->names = ( char** ) malloc(cap * sizeof( void * ));
  arr->cap = cap;
  arr->len = 0;
}
 
void free_all(struct str_arr* arr) {
  if (arr->cap > 0) {
    free(arr->names);
  }
  arr->len = 0;
  arr->cap = 0;
}

/* arr => arr2 */
void copy(struct str_arr* arr, struct str_arr* arr2) {
  for(char i=0; i<arr->len; i++) {
    arr2->names[i] = arr->names[i];
    arr2->len = arr2->len >= arr->len ? arr2->len : arr->len;
  }
}

void duplicate_mem(struct str_arr* arr) {
  if (arr->len <= 0 || arr->cap <= 0) {
    allocate(arr, 5);
    return;
  }
    
  struct str_arr* prov;
  prov = (struct str_arr*) malloc(sizeof(void*));

  unsigned prov_cap = arr->cap;

  allocate(prov, arr->cap);
  copy(arr, prov);

  free_all(arr);

  allocate(arr, prov_cap * 2);
  copy(prov, arr);
}

  
void append(struct str_arr* arr, char* name) {
  if (arr->len + 1 > arr->cap) {
    duplicate_mem(arr);
  }
  arr->names[arr->len] = name;
  arr->len += 1;
}

void printarr(struct str_arr* arr) {
  if (arr->len <= 0) {
    printf("array vazio :(\n");
    return;
  }
  printf("ARRAY: [");
  for (int i=0; i<arr->len - 1; i++) {
    printf(" \"%s\",", arr->names[i]);
  }
  printf(" \"%s\" ]\n", arr->names[arr->len - 1]);

}

void remove_end(struct str_arr* arr) {
  if (arr->len >0)
    arr->len -= 1;
}

bool remove_elem(struct str_arr* arr, unsigned i) {
  if (arr->len <= i) return false;
  arr->len -= 1;
  for (char j=i; j<arr->len; j++) {
    arr->names[j] = arr->names[j + 1];
  }
  return true;
}

bool insert_elem(struct str_arr* arr, char* name, unsigned i) {
  if (arr->len <= i) return false;
  if (arr->len + 1 >= arr->cap) duplicate_mem(arr);
  for (char j=arr->len - 1; j>0; j--) {
    arr->names[j + 1] = arr->names[j];
  }
  arr->names[i] = name;
  arr->len += 1;
  return true;
}

/*
int main() {
  struct str_arr* lu_names;

  lu_names = (struct str_arr*) malloc(sizeof(void*));
  allocate(lu_names, 5);
  append(lu_names, "yyyyyyy3so");
  printarr(lu_names);
  printf("cap %d\n", lu_names->cap);
  printf("len %d\n", lu_names->len);
  append(lu_names, "yyyyyyy3so");
  printarr(lu_names);
  printf("cap %d\n", lu_names->cap);
  printf("len %d\n", lu_names->len);
  append(lu_names, "yyyyyyy3so");
  printarr(lu_names);
  printf("cap %d\n", lu_names->cap);
  printf("len %d\n", lu_names->len);
  append(lu_names, "yyyyyyy3so");
  printarr(lu_names);
  printf("cap %d\n", lu_names->cap);
  printf("len %d\n", lu_names->len);
  append(lu_names, "yyyyyyy3so");
  printarr(lu_names);
  printf("cap %d\n", lu_names->cap);
  printf("len %d\n", lu_names->len);

  insert_elem(lu_names, "luuukkkkk", 2);
  printarr(lu_names);
  printf("cap %d\n", lu_names->cap);
  printf("len %d\n", lu_names->len);
  insert_elem(lu_names, "luuukkkkk", 2);
  printarr(lu_names);
  printf("cap %d\n", lu_names->cap);
  printf("len %d\n", lu_names->len);
  insert_elem(lu_names, "luuukkkkk", 2);
  printarr(lu_names);
  printf("cap %d\n", lu_names->cap);
  printf("len %d\n", lu_names->len);

  remove_end(lu_names);
  printarr(lu_names);
  printf("cap %d\n", lu_names->cap);
  printf("len %d\n", lu_names->len);

  remove_elem(lu_names, 4);
  printarr(lu_names);
  printf("cap %d\n", lu_names->cap);
  printf("len %d\n", lu_names->len);
  remove_elem(lu_names, 4);
  printarr(lu_names);
  printf("cap %d\n", lu_names->cap);
  printf("len %d\n", lu_names->len);
  remove_elem(lu_names, 4);
  printarr(lu_names);
  printf("cap %d\n", lu_names->cap);
  printf("len %d\n", lu_names->len);

  remove_elem(lu_names, 4);
  printarr(lu_names);
  printf("cap %d\n", lu_names->cap);
  printf("len %d\n", lu_names->len);
  
  printf("KKKKKKKKKKKKKKKKKKKK\n");

  return 0;
}
*/
