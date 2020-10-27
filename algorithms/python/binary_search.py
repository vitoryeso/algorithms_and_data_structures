def binary_search(A, p, r, v):
    if r > p:
        q = int( (r + p)/2 )
        if v > A[q]:
            return binary_search(A, q + 1, r, v)
        else:
            return binary_search(A, p, q, v)
    if v == A[p]:
        return p
    else: 
        return None
