from .binary_search import binary_search
def match_sum(S, n, x):
    for i in range(0, n ):
        c = x - S[i]
        if binary_search(S, i + 1, n + 1, c):
            return True
    return False

