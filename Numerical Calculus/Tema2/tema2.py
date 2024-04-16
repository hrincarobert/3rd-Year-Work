import numpy as np
import sys
global L1,U1

def has_LU_decomposition(A):
    n = len(A)
    if A.shape[0] != A.shape[1]:
        print("Not fit for LU.")
        return False  
    

    for k in range(1, n + 1):
        minor = A[:k, :k]
        if np.linalg.det(minor) == 0:
            print("Not fit for LU.")
            return False  

    return True

def lu_decomposition(A):

   
    n = A.shape[0]
    L = np.zeros((n, n))
    U = np.zeros((n, n))

    for k in range(n):
        U[k, k] = 1
        for j in range(k, n):
            sum0 = sum(L[k, s] * U[s, j] for s in range(k))
            L[j, k] = A[j, k] - sum0

        for j in range(k, n):
            sum1 = sum(L[k, s] * U[s, j] for s in range(k))
            U[k, j] = (A[k, j] - sum1) / L[k, k]


    
    return L, U

def combine_LU(L, U):
  
    n = L.shape[0]
    combined_matrix = np.zeros((n, n))

    for i in range(n):
        for j in range(n):
            if i <= j:
                combined_matrix[i, j] = U[i, j]
            else:
                combined_matrix[i, j] = L[i, j]

    n1 = L.shape[0]
    for i in range(n1):
        combined_matrix[i, i] = L[i, i]

    return combined_matrix

def determinant_LU(L):
    return np.prod(np.diag(L))


def euclidean_norm(x, y):
    return np.linalg.norm(x - y)


def forward_substitution(L, b):
  
    n = L.shape[0]
    x = np.zeros(n)

    for i in range(n):
        x[i] = b[i]
        for j in range(i):
            x[i] -= L[i, j] * x[j]
        x[i] /= L[i, i]

    return x

def backward_substitution(U, b):
   
    n = U.shape[0]
    x = np.zeros(n)

    for i in range(n - 1, -1, -1):
        x[i] = b[i]
        for j in range(i + 1, n):
            x[i] -= U[i, j] * x[j]
        x[i] /= U[i, i]

    return x


A_init = np.array([[0, 2, 2], [5, 0, 4], [5, 6, 6.5]])
b = np.array([2, 2, 2])

ok = has_LU_decomposition(A_init)

if ok == False:
    sys.exit()


L,U= lu_decomposition(A_init)

A = combine_LU(L,U)
det_A = determinant_LU(A)

x1= np.linalg.solve(A_init, b)
norm_error = euclidean_norm(np.dot(A_init, x1), b)

y = forward_substitution(L, b)
x = backward_substitution(U, y)

euclidean_norm_x = np.linalg.norm(x - x1)

A_inv = np.linalg.inv(A)

difference_vector = x1 - np.dot(A_inv, b)


#euclidean_norm_matrix = np.linalg.norm(difference_vector)
euclidean_norm_matrix = euclidean_norm(x1 , np.dot(A_inv, b))

print("LU Decomposition:")
print(A)
print("\nDeterminant of A:", det_A)
print("\nSolution x using forward,backward substitution:", x)
print("\nSolution x using numpy:", x1)
print("\nEuclidean Norm of A*x - b:", norm_error)
print("\nEuclidean Norm of x_lu - x_lib:", euclidean_norm_x)
print("\nEuclidean Norm of matrices:", euclidean_norm_matrix)
