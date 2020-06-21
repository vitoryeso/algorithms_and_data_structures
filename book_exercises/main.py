from luAlgorithm import binary_search, merge_sort, selection_sort, insertion_sort, recursive_insertion_sort, match_sum

a = [1,4,3,56,7,34,3,6,2,3,5,11,2]
print("a: ", a)
insertion_sort(a)
print("a sorted with insertion: ", a, end="\n\n")

a = [1,4,3,56,7,34,3,6,2,3,5,11,2]
print("a: ", a)
recursive_insertion_sort(a, len(a) - 1)
print("a sorted with recursive insertion: ", a, end="\n\n")

a = [1,4,3,56,7,34,3,6,2,3,5,11,2]
print("a: ", a)
selection_sort(a)
print("a sorted with selection: ", a, end="\n\n")

a = [1,4,3,56,7,34,3,6,2,3,5,11,2]
print("a: ", a)
merge_sort(a, 0, len(a) - 1)
print("a sorted with merge: ", a, end="\n\n")

print("a: ", a)
i = binary_search(a, 0, len(a) - 1, 6)
print("search index of 56 value using binary_search: ", i, end="\n\n")

print("exist A[i] + A[j] == x, where x is: ", 8)
print("yes" if match_sum(a, len(a) - 1, 8) else "no")

print("exist A[i] + A[j] == x, where x is: ", 0)
print("yes" if match_sum(a, len(a) - 1, 0) else "no")
