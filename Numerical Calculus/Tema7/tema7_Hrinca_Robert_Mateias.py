import numpy as np
import sys
import random
from functools import reduce

epsilon = 1e-6
k_max = 1000

def metoda_horner(coefs, x):
    return reduce(lambda b, a: b * x + a, coefs)


def metoda_muller(coefs):
    epsilon = 1e-10
    k_max = 1000
    delta_x = 1
    k = 0
    a0 = abs(coefs[0])
    A = max(abs(c) for c in coefs[1:])
    r = (a0 + A) / a0
    
    random_x = lambda: random.uniform(-r, r)
    x0, x1, x2 = random_x(), random_x(), random_x()
    
    metoda_horner = lambda x: \
        reduce(lambda b, a: b * x + a, coefs)
    
    while True:
        h0 = x1 - x0
        h1 = x2 - x1
        eta0 = (metoda_horner(x1) - metoda_horner(x0)) / h0
        eta1 = (metoda_horner(x2) - metoda_horner(x1)) / h1
        a = (eta1 - eta0) / (h1 + h0)
        b = eta1 + h1 * a
        c = metoda_horner(x2)
        
        discriminant = b**2 - 4*a*c
        if discriminant < 0:
            return None
        
        ok1 = lambda b, discriminant, epsilon: None if abs(b + np.sign(b) * np.sqrt(discriminant)) < epsilon else None
        calc_Delta = lambda b, c, discriminant: 2 * c / (b + np.sign(b) * np.sqrt(discriminant))

        delta_x = calc_Delta(b, c, discriminant)
        x3 = x2 - delta_x
        k += 1
        
        if abs(delta_x) < epsilon or k == k_max or abs(delta_x) >= 10**8:
            break
        
        x0, x1, x2 = x1, x2, x3
    
    if abs(delta_x) < epsilon:
        return x3
    else:
        print("Divergence occurred")
        return None


#coefs = [1, -6, 11, -6]
#coefs = [42, -55, -42, 49, -6]
coefs = [8, -38, 49 ,-22, 3]

radacini = []
for _ in range(10):
    root = metoda_muller(coefs)
    if root is not None and all(abs(root - r) > epsilon for r in radacini):
        radacini.append(root)

print("Radacini gasite:")
for radacina in radacini:
    print(radacina)


with open('radacini.txt', 'w') as f:
    for radacina in radacini:
        f.write(f"{radacina}\n")

print("Radacini distincte salvate in 'radacini.txt'")
