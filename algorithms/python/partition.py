from .swap import swap

def partition(A, p, r):
    # A[r] is the pivot
    i = p - 1
    for j in range(p, r):
        if A[j] < A[r]:
            swap(A, i + 1, j)
            i += 1
    swap(A, i + 1, r)
    return i + 1

