from copy import copy
from luAlgorithm import binary_search, merge_sort, selection_sort, insertion_sort, recursive_insertion_sort, match_sum, merge

k = 6

def merge_modified(A, p, r):
    if r > p + 1:
        q = int((r + p) / 2)
        merge_modified(A, p, q)
        merge_modified(A, q + 1, r)
        global k
        if (r - p) < k:
            insertion_sort(A, p, r)
        else:
            merge.merge(A, p, q, r)
 
A = [1,4,3,56,7,34,3,6,2,3,5,11,2]
print("A: ", A)
insertion_sort(A, 0, len(A) - 1)
print("A sorted with insertion: ", A, end="\n\n")

A = [1,4,3,56,7,34,3,6,2,3,5,11,2]
print("A: ", A)
recursive_insertion_sort(A, len(A) - 1)
print("A sorted with recursive insertion: ", A, end="\n\n")

A = [1,4,3,56,7,34,3,6,2,3,5,11,2]
print("A: ", A)
selection_sort(A)
print("A sorted with selection: ", A, end="\n\n")

A = [1,4,3,56,7,34,3,6,2,3,5,11,2]
print("A: ", A)
merge_sort(A, 0, len(A) - 1)
print("A sorted with merge: ", A, end="\n\n")

A = [1,4,3,56,7,34,3,6,2,3,5,11]
print("A: ", A)
merge_modified(A, 0, len(A) - 1)
print("A sorted with merge_modified: ", A, end="\n\n")

print("A: ", A)
i = binary_search(A, 0, len(A) - 1, 6)
print("search index of 6 value using binary_search: ", i, end="\n\n")

print("exist A[i] + A[j] == x, where x is: ", 8)
print("yes" if match_sum(A, len(A) - 1, 8) else "no")

print("exist A[i] + A[j] == x, where x is: ", 0)
print("yes" if match_sum(A, len(A) - 1, 0) else "no")
