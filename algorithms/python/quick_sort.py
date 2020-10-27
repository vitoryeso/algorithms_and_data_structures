from .partition import partition

def quick_sort(A, p, r):
    if r > p:
        q = partition(A, p, r)
        quick_sort(A, p, q - 1)
        quick_sort(A, q + 1, r)
