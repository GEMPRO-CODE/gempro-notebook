from random import randrange as rr, randint as ri, shuffle as sh, sample as sm
from string import ascii_lowercase as alpha
from itertools import combinations as combs

def out(*a): print(" ".join(map(str, a)))

def outa(a): print(" ".join(map(str, a)))

def tree(n): return [[rr(1, i), i] for i in range(2, n + 1)]

def graph(n, m): return sm(list(combs(range(1, n + 1), 2)), m)

def arr(n, l = 0, r = 10 ** 9): return [ri(l, r) for i in range(n)]

def darr(n, l = 0, r = 10 ** 9): return sm(range(l, r), n)

def perm(n): p = list(range(1, n + 1)); sh(p); return p
