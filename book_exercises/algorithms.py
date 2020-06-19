def merge(A, p, q, r): 
    n1 = q - p
    n2 = r - (q + 1)
    C = [ A[p + i] for i in range(n1 + 1) ]
    D = [ A[q + 1 + i] for i in range(n2 + 1) ]
    print("C: ", C)
    print("D: ", D)
    C.append(999999999)
    D.append(999999999)
    i = j = 0
    for k in range(p, r):
        if C[i] >= D[j]:
            A[k] = D[j]
            j += 1
        else:
            A[k] = C[i]
            i += 1

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

def inserction_sort(A):
    for j in range(1, len(A)):
        key = A[j]
        i = j - 1
        while key < A[i] and i >= 0:
            A[i + 1] = A[i]
            i -= 1
        A[i + 1] = key

A = [1,4,3,56,7,34,3,6,2,3,5,11,2]

print("A: ", A)
inserction_sort(A)
print("A sorted with inserction: ", A)
A = [1,4,3,56,7,34,3,6,2,3,5,11,2]
selection_sort(A)
print("A sorted with selection: ", A)

A = [2,321,432,4,32,1,2,3,4,5,3,5,7,18,99,0,23,11]
# p = 5; q = 9; r = 14
answer =[2,321,432,4,32,1,2,3,3,4,5,5,7,18,99,0,23,11] 
print("input: {}, 5, 9, 14".format(A))
print("desired output: ", answer)
merge(A, 5, 9, 14)
print("output: ", A)
