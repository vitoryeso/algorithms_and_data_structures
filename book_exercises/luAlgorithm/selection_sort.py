def selection_sort(A):
    for i in range(len(A) - 1):
        min_ = A[i]
        id_ = i
        for j in range(i, len(A)):
            if A[j] < min_:
                min_ = A[j]
                id_ = j
        A[id_] = A[i]
        A[i] = min_
