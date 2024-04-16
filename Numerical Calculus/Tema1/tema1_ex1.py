def ex1():
    u = 1.0
    m = 0
    x = 10.0

    while (1 + x ** -m) != 1:
        m += 1
        u = x ** -m
    u = u*10
    print("Rezultatul este", u)

ex1()
