import math
import numpy
import copy

def matrice_1(n):
     return [[1 if i == j else 0 for j in range(n)] for i in range(n)]


def calculeaza_p_q(A, n):
    p, q = max((i, j) for i in range(n) for j in range(i+1, n) if abs(A[i][j]) >= max(abs(A[p][q]) for p in range(n) for q in range(p+1, n)))
    return p, q

def get_diagonal_matrix(A, n):
    return [[A[i][i] if i == j else 0 for j in range(n)] for i in range(n)]


def calculeaza_unghi_t(A, p, q):

    if A[p][p] == A[q][q]:
        if A[p][q] > 0:
            return math.pi / 4
        else:
            return - math.pi / 4
    else:
        return 1 / 2 * math.tan(2 * A[p][q] / (A[p][p] - A[q][q]))


def is_diagonal(A):

    i = 0
    while i < len(A):
        j = 0
        while j < len(A):
            if i != j and abs(A[i][j]) > 10 ** (-8):
                return True
            j += 1
        i += 1
    return False


def R_pq_function(n, p, q, c, s):
    
    R_pq = [[1 if i == j and i != p and i != q else
             c if i == p == j or i == q == j else
             s if i == p and j == q else
             -s if i == q and j == p else
             0 for j in range(n)] for i in range(n)]
    return R_pq


def jacobi_alg(A, n):
   
    k = 0
    k_max = 1000
    U = matrice_1(n)
    p, q= calculeaza_p_q(A, n)
    unghi_t = calculeaza_unghi_t(A, p, q)
    c = math.cos(unghi_t)
    s = math.sin(unghi_t)
    iteratie_finala = 0

    while k <= k_max and is_diagonal(A) is True:
        R_pq = R_pq_function(n, p, q, c, s)
        A = numpy.array(A)

        # Calcul A, U
        R_pq = numpy.array(R_pq)
        R_pq_transpusa = numpy.transpose(R_pq)
        U_x_A = numpy.dot(R_pq, A)
        A = numpy.dot(U_x_A, R_pq_transpusa)
        U = numpy.dot(U, R_pq_transpusa)

        # Recalcul teta

        p, q= calculeaza_p_q(A, n)
        
        unghi_t = calculeaza_unghi_t(A, p, q)
        c = math.cos(unghi_t)
        s = math.sin(unghi_t)
        iteratie_finala = k
        k = k + 1

    print("iteratie finala:", iteratie_finala)
    return A, U




def fact_choleski(A):
    max_iterations = 1000
    tolerance = 1e-8

    L = numpy.linalg.cholesky(A)
    L_transpusa = numpy.transpose(L)
    A_0 = numpy.dot(L, L_transpusa)
    A_1 = numpy.dot(L_transpusa, L)

    def iterate(L, L_transpusa, A_0, A_1):
        k = 0
        while k <= max_iterations and numpy.linalg.norm(A_1 - A_0) > tolerance:
            A_0 = numpy.dot(L_transpusa, L)
            L = numpy.linalg.cholesky(A_1)
            L_transpusa = numpy.transpose(L)
            A_1 = numpy.dot(L, L_transpusa)
            k += 1
        print("Final iteration:", k)
        return A_1

    resuL_transpusa = iterate(L, L_transpusa, A_0, A_1)
    return resuL_transpusa




def point_1():

    print("### 1 ###")
    print("\n")

  
    n = 4

    A = [[1, 2, 3, 1],
         [4, 5, 6, 4],
         [7, 8, 9, 5],
         [1, 5, 7, 4]]
    
    A_initial = copy.deepcopy(A)

    rezuL_transpusaat = jacobi_alg(A, n)
    A , U = rezuL_transpusaat

    ## A final 

    U_transpusa = numpy.transpose(U)
    U_x_A = numpy.dot(U_transpusa, numpy.array(A_initial))
    A_final = numpy.dot(U_x_A, U)

    print(A_final)

    ## norma

    diag_matrix = get_diagonal_matrix(A, n)
    a = numpy.dot(A_initial, U)
    b = numpy.dot(U, diag_matrix)
    norma = numpy.linalg.norm(a - b)
    print("Norma:", norma)
    print("\n")  



def point_2():

    print("### 2 ###")

    A = [[42, 21, 80],
         [12, 18, 10],
         [18, 10, 21]]

    A_factorizat = fact_choleski(A)
    print(A_factorizat, "\n")



def point_3():
    print("### 3 ###")

    A = [[4, 2],
         [2, 10],
         [8, 10]]

    U, S, V = numpy.linalg.svd(A, full_matrices=True)

    print("Valorile singulare :", S)
    rang = numpy.linalg.matrix_rank(A)
    print("\n")
    print("Rangul matricei:", rang)
    conditionare = numpy.linalg.cond(A)
    print("\n")
    print("Numarul de conditionare :", conditionare)
    A_I = numpy.linalg.pinv(A)
    print("\n")
    print("Pseudoinversa Moore-Penrose :")
    print("\n")
    print(A_I)
    A_transpusa = numpy.transpose(A)
    print("\n")  

    A_J = numpy.dot(numpy.linalg.inv(numpy.dot(A_transpusa, A)), A_transpusa)
    print("Pseudoinversa in sensul celor mai mici patrate:")
    print("\n")  
    print(A_J)
    print("\n")  
    print("Norma A_I, A_J:", numpy.linalg.norm(A_I - A_J))

point_1()
point_2()
point_3()

