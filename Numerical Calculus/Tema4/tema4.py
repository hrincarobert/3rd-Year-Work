
import ast
import numpy as np
import math


def div_ok(nr):
    if abs(nr) <= 10 ** (-6):
        raise ValueError("Cant devide")

def calcul_norma(x, y):
    squared_diff_sum = sum((x_val - y_val) ** 2 for x_val, y_val in zip(x, y))
    norm = math.sqrt(squared_diff_sum)
    return norm

def multiply_matrix_vector(matrix, vector):
    result = []
    for row in matrix:
        dot_product = sum(row_val * vector[col_ind_mat] for row_val, col_ind_mat in row)
        result.append(dot_product)
    return result



def citire_matrice(fis1):
    with open(fis1, "r") as file_in:
        lines = file_in.readlines()

    n = int(lines[0].strip())
    a = [[] for _ in range(n)]

    for line in lines[1:]:
        elemente = line.strip().split(",")
        if len(elemente) > 1:
            nr = float(elemente[0].strip())
            if nr == 0:
                continue
            i = int(elemente[1].strip())
            j = int(elemente[2].strip())
            found = False
            for k, elem in enumerate(a[i]):
                if elem[1] == j:
                    a[i][k][0] += nr
                    found = True
                    break
            if not found:
                a[i].append([nr, j])
                a[i].sort(key=lambda x: x[1])

    return n, a



def citire_sistem(fis1, fis2):
    with open(fis1, "r") as file_in:
        lines = file_in.readlines()

    n = int(lines[0].strip())
    a = [[] for _ in range(n)]
    b = []

    for line in lines[1:]:
        elemente = line.strip().split(",")
        if len(elemente) > 1:
            nr = float(elemente[0].strip())
            if nr == 0:
                continue
            i = int(elemente[1].strip())
            j = int(elemente[2].strip())
            found = False
            for k, elem in enumerate(a[i]):
                if elem[1] == j:
                    a[i][k][0] += nr
                    found = True
                    break
            if not found:
                a[i].append([nr, j])
                a[i].sort(key=lambda x: x[1])

    lines2 = []
    with open(fis2, "r") as file_in:
        for line in file_in:
            lines2.append(line)
    for ind_mat in range(0, n):
        nr = float(lines2[ind_mat])
        b.append(nr)
    return n, a, b


def zero_diag(a, n):
    for i in range(1, n):
        found_nonzero = any(elem[0] != 0 for elem in a[i] if elem[1] == i)
        if not found_nonzero:
            return False
    return True


def gauss_seidel(a, b, n):

    eps = 1e-6
    k_max = 10000
    xc = [0.0] * n
    xp = [0.0] * n

    def update_xc():
        nonlocal xc, xp
        for i in range(n):
            sum1 = sum2 = 0.0
            element_diagonal = 0.0
            for value, ind_mat in a[i]:
                if ind_mat < i:
                    sum1 += value * xc[ind_mat]
                elif ind_mat == i:
                    element_diagonal = value
                elif ind_mat > i:
                    sum2 += value * xp[ind_mat]
            if element_diagonal == 0:
                raise ValueError("Diagonal element is zero")
            xc[i] = (b[i] - sum1 - sum2) / element_diagonal

    def calcul_norma(vec1, vec2):
        return max(abs(x - y) for x, y in zip(vec1, vec2))

    k = 0
    while k < k_max:
        update_xc()
        if calcul_norma(xc, xp) < eps:
            return xc
        xp = xc.copy()
        k += 1

    return False



def verif_egalitate_matrice(matrice1, matrice2):
    n = len(matrice1)
    ok = 1
    i = 0
    while i < n and ok == 1:
        j = 0
        while j < len(matrice1[i]) and ok == 1:
            k = 0
            while k < len(matrice2[i]) and ok == 1:
                if matrice1[i][j][1] == matrice2[i][k][1]:
                    if abs(matrice1[i][j][0] - matrice2[i][k][0]) >= 10 ** (-6):
                        ok = 0
                k += 1
            j += 1
        i += 1

    if ok == 1:
        print("Matrice egale")
    else:
        print("Matricele nu sunt egale")


def adunare_matrice(matrice1, matrice2):
    n = len(matrice1)
    new_matrix = [[] for _ in range(n)]

    for i in range(n):
        combined_elemente = {}
        for elem in matrice1[i]:
            combined_elemente[elem[1]] = combined_elemente.get(elem[1], 0) + elem[0]
        for elem in matrice2[i]:
            combined_elemente[elem[1]] = combined_elemente.get(elem[1], 0) + elem[0]

        new_matrix[i] = [[value, ind_mat] for ind_mat, value in combined_elemente.items()]

    return new_matrix


def rez_sistem(fis1, fis2):

    n, a, b = citire_sistem(fis1, fis2)
    if zero_diag(a, n):

        xc = gauss_seidel(a, b, n)

        if xc == False:
            print("Divergenta")
        else:
            print(xc)
            xc_norma = multiply_matrix_vector(a, xc)
            norma = calcul_norma(xc_norma, b)
            print("Norma ->", norma)
    else:
        print("Nu se poate rezolva")

def rez_matrice(matrice1, matrice2, matrice3):

    _, a = citire_matrice(matrice1)
    _, b = citire_matrice(matrice2)
    _, aplusb_fis = citire_matrice(matrice3)
    aplusb = adunare_matrice(a, b)

    verif_egalitate_matrice(aplusb, aplusb_fis)


#rez_sistem("sisteme/a_1.txt", "sisteme/b_1.txt")
        
#rez_matrice("sisteme/a.txt", "sisteme/b.txt", "sisteme/aplusb.txt")

