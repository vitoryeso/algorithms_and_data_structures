def merge_2(A, p, q, r):
    n1 = q - p
    n2 = r - (q + 1)
    C = [ A[p + i] for i in range(n1 + 1) ]
    D = [ A[q + 1 + i] for i in range(n2 + 1) ]
    k = p
    i = j = 0
    while k <= r and i <= n1 and j <= n2:
        if C[i] >= D[j]:
            A[k] = D[j]
            j += 1
        else:
            A[k] = C[i]
            i += 1
        k += 1
    if k <= r:
        if j > n2:
            for k_ in range(k, r+1):
                A[k_] = C[i]
                i += 1
        else:
            for k_ in range(k, r+1):
                A[k_] = D[j]
                j += 1
        
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

def merge_sort_2(A, p, r):
    if p < r:
        q = int((p + r)/2)
        merge_sort_2(A, p, q)
        merge_sort_2(A, q + 1, r)
        merge_2(A, p, q, r)

def merge_sort(A, p, r):
    if p < r:
        q = int((p + r)/2)
        merge_sort(A, p, q)
        merge_sort(A, q + 1, r)
        merge(A, p, q, r)

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

def insertion_sort(A):
    for j in range(1, len(A)):
        key = A[j]
        i = j - 1
        while key < A[i] and i >= 0:
            A[i + 1] = A[i]
            i -= 1
        A[i + 1] = key

def recursive_insertion_sort(A, n):
    if n > 0:
        recursive_insertion_sort(A, n - 1)
        key = A[n]
        i = n - 1
        while A[i] > key and i >=0:
            A[i + 1]  = A[i]
            i -= 1
        A[i + 1] = key

A = [1,4,3,56,7,34,3,6,2,3,5,11,2]
print("A: ", A)
insertion_sort(A)
print("A sorted with insertion: ", A, end="\n\n")

A = [1,4,3,56,7,34,3,6,2,3,5,11,2]
print("A: ", A)
recursive_insertion_sort(A, len(A) - 1)
print("A sorted with recursive insertion: ", A, end="\n\n")

A = [1,4,3,56,7,34,3,6,2,3,5,11,2]
print("A: ", A)
selection_sort(A)
print("A sorted with selection: ", A, end="\n\n")

A = [1,4,3,56,7,34,3,6,2,3,5,11,2]
print("A: ", A)
merge_sort(A, 0, len(A) - 1)
print("A sorted with merge: ", A, end="\n\n")

A = [1,4,3,56,7,34,3,6,2,3,5,11,2]
print("A: ", A)
merge_sort_2(A, 0, len(A) - 1)
print("A sorted with merge_2: ", A, end="\n\n")

