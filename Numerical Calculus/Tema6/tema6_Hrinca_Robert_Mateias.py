import numpy as np



def newton_func(xi, yi, xp):
    n = len(xi) - 1
    h = xi[1] - xi[0]
    u = (xp - xi[0]) / h
    result = yi[0]

    result += sum((lambda i: (1 / np.math.factorial(i)) * np.prod([(u - j) for j in range(i)]) * diferente_finite(yi, i, 0))(i) for i in range(1, n + 1))
    
    return result


def diferente_finite(y, k, i):
    if k == 1:
        return y[i+1] - y[i]
    return diferente_finite(y, k - 1, i + 1) - diferente_finite(y, k - 1, i)

def metoda_aikten(xi, yi, xp):
    n = len(xi)
    delta = np.zeros((n, n))
    [lambda i, j: yi[i] if j == 0 else (delta[i+1][j-1] - delta[i][j-1]) / (xi[i+j] - xi[i]) for i in range(n) for j in range(n)]
    
    result = yi[0]
    u = (xp - xi[0]) / (xi[1] - xi[0])
    [lambda i: result + np.prod([(u - j) * delta[0][i] for j in range(i)]) for i in range(1, n)]
    
    return result

def metoda_horner(polinom, x):
    rezultat = polinom[0]
    for i in range(1, len(polinom)):
        rezultat = rezultat * np.float64(x) + polinom[i]
    return rezultat


def interpolare_patrate(x_barat, x, y, n, m):
   
    B = [[sum(x[k] ** (i + j) for k in range(n)) for j in range(m + 1)] for i in range(m + 1)]
   
    f = [sum(x[k] ** i * y[k] for k in range(n)) for i in range(m + 1)]
   
    a = np.linalg.solve(B, f)

    metoda_horner = lambda coeffs, x: sum(coeff * (x ** i) for i, coeff in enumerate(reversed(coeffs)))

    f_de_x_barat = metoda_horner(a, x_barat)

    return f_de_x_barat


def func(x):
    return np.sin(x)


n = 4

xi = [2 , 4 , 6,8]

yi = [150, 147, 120, 300]

xp = 2.5

rez_newton = newton_func(xi, yi, xp)
print("Newton:", rez_newton)

rez_aikten = metoda_aikten(xi, yi, xp)
print("Aitken :", rez_aikten)

m=5

rez_ls = interpolare_patrate(xp, xi, yi, n, m)
print("Metoda celor mai mici patrate :", rez_ls)

f_xp = func(xp)
diff_newton = abs(rez_newton - f_xp)
diff_aitken = abs(rez_aikten - f_xp)

print("Norma Newton:", diff_newton)
print("Norma Aitken:", diff_aitken)

diff_least = abs(rez_ls - f_xp)
print("Norma metoda celor mai mici patrate:", diff_least)

