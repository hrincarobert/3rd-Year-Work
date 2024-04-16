import numpy as np


def compute_vector_b(A, s):
    n, _ = A.shape
    b = [sum(s[j] * A[i][j] for j in range(n)) for i in range(n)]
    return b


def qr_householder(A):
    m, n = A.shape
    R = A.copy()
    Q = np.identity(m)
    for k in range(n):
        # reflection vector
        x = R[k:, k]
        e = np.zeros_like(x)
        e[0] = np.sign(x[0])
        v = np.linalg.norm(x) * e + x #householder ref. vector
        v = v / np.linalg.norm(v)

        # reflection to R
        R[k:, :] -= 2 * np.outer(v, v.T @ R[k:, :])

        # reflection to Q
        Q[:, k:] -= 2 * Q[:, k:] @ np.outer(v, v.T)

    return Q, R


def solve_qr(Q, R, b):
    y = Q.T @ b
    x = np.linalg.solve(R, y)
    return x


def inverse_qr(A):
    Q, R = np.linalg.qr(A)
    n = A.shape[0]
    A_inv = np.zeros((n, n))
    for i in range(n):
        e = np.zeros(n)
        e[i] = 1
        y = Q.T @ e
        x = np.linalg.solve(R, y)
        A_inv[:, i] = x
    return A_inv




A = np.array([
        [1, 1, 4],
        [2, 2, 8],
        [0, 1, 2]
])
A = A.astype('float64')
s = np.array([3, 2, 1])

#n = 3

#A = np.random.uniform(-10, 10, size=(n, n))
#
#while np.linalg.det(A) == 0:
#    A = np.random.uniform(-10, 10, size=(n, n))
#s = np.random.uniform(-10, 10, size=n)
    


# 1.compute vector b

b = compute_vector_b(A, s)
print("### 1. Vector b ###")
print("b = ", b)


# 2.QR decomposition

print("\n")
print("### 2. QR decomposition ###")
Q, R = qr_householder(A)
Q_real, R_real = np.linalg.qr(A)
print("Q")
print(Q)
print("Q Real")
print(Q_real)
print("R")
print(R)
print("R Real")
print(R_real)


# 3. Ax=b

x_householder = solve_qr(Q, R, b)
x_qr = solve_qr(Q_real, R_real, b)

print("\n")
print("### 3. Ax=b ###")
print("X_Householder:", x_householder)
print("X_qr", x_qr)
print("|x_qr-x_householder| : ", np.linalg.norm(x_householder - x_qr))

# 4
print("\n")
print("### 4. Eucledian norms ###")
print("|A*x_householder - b| = ", np.linalg.norm(np.dot(A, x_householder) - b))
print("|A*x_qr - b| = ", np.linalg.norm(np.dot(A, x_householder) - b))
print("|(x_householder -s) / s| = ", np.linalg.norm(x_householder - s) / np.linalg.norm(s))
print("|(x_qr - s) / s| = ", np.linalg.norm(x_qr - s) / np.linalg.norm(s))
1
# 5

print("\n")
print("### 5. Inverse matrix ###")
A_householder = inverse_qr(A)
print(A_householder)
A_library = np.linalg.inv(A)
print(A_library)
print("\n")
print("|A^(-1) householder - A^(-1) library| = ", np.linalg.norm(A_householder - A_library))
print("\n")