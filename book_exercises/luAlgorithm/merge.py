def merge(A, p, q, r):
    n1 = q - p
    n2 = r - (q + 1)
    C = [ A[p + i] for i in range(n1 + 1) ]
    D = [ A[q + 1 + i] for i in range(n2 + 1) ]
    C.append(999999999)
    D.append(999999999)
    i = j = 0
    for k in range(p, r + 1):
        if C[i] >= D[j]:
            A[k] = D[j]
            j += 1
        else:
            A[k] = C[i]
            i += 1
     

