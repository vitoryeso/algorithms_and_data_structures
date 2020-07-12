def counting_sort(A, k):
    C = [0]*(k + 1)
    B = [0]*len(A)
    for a in A:
        C[a] += 1
    for i in range(1, k + 1):
        C[i] += C[i - 1]
    for a in reversed(A):
        B[C[a] - 1] = a
        C[a] -= 1
    return B
