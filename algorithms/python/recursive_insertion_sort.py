def recursive_insertion_sort(A, n):
    if n > 0:
        recursive_insertion_sort(A, n - 1)
        key = A[n]
        i = n - 1
        while A[i] > key and i >=0:
            A[i + 1]  = A[i]
            i -= 1
        A[i + 1] = key
