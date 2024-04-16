import random

def ex1():
    u = 1.0
    m = 0
    x = 10.0

    while (1 + x ** -m) != 1:
        m += 1
        u = x ** -m
    u = u * 10
    print("Prezicia masinii este :", u)
    return u

u = ex1()

def ex2_prefedined_addition():
    print("Rezultate pentru predefinite x, y, z pentru adunare:")

    x = 1.0
    y = u / 10
    z = u / 10



    result1 = (x + y) + z
    result2 = x + (y + z)


    if result1 != result2:
        print(f"Rezultatul (x + y) + z: {result1}")
        print(f"Rezultatul x + (y + z): {result2}")
    else:
        print("Rezultatele sunt egale.")

ex2_prefedined_addition()

print("\n")

def ex2_randomized_mutiplication():

    print("Rezultatele random x, y, z pentru inmultire:")

    while True:
    
        x = random.uniform(0, 10)
        y = random.uniform(0, 10)
        z = random.uniform(0, 10)

        result3 = (x * y) * z
        result4 = x * (y * z)

        if result3 != result4:
            print(f"Rezultatul (x * y) * z: {result3}")
            print(f"Rezultatul x * (y * z): {result4}")
            break 
        else:
            print("Rezultatele sunt egale.")
    
ex2_randomized_mutiplication()