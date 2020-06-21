def insertion_sort(A):
    for j in range(1, len(A)):
        key = A[j]
        i = j - 1
        while key < A[i] and i >= 0:
            A[i + 1] = A[i]
            i -= 1
        A[i + 1] = key
