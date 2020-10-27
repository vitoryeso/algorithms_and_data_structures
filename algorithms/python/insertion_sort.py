def insertion_sort(A, p, r):
    for j in range(p, r  + 1):
        key = A[j]
        i = j - 1
        while key < A[i] and i >= 0:
            A[i + 1] = A[i]
            i -= 1
        A[i + 1] = key
