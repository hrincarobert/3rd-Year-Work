import math
import random

err_list = []

def T_4(a):
    rezultat=(105*a-10*a**3)/(105-45*a*a+a**4)
    return rezultat 

def T_5(a):
    rezultat=(945*a-105*a**3+a**5)/(945-420*a*a+15*a**4)
    return rezultat 

def T_6(a):
    rezultat=(10395*a-1260*a**3+21*a**5)/(10395-4725*a*a+210*a**4-a**6)
    return rezultat 

def T_7(a):
    rezultat=(135135*a-17325*a**3+378*a**5-a**7)/(135135-62370*a*a+3150*a**4-28*a**6)
    return rezultat 

def T_8(a):
    rezultat=(2027025*a-270270*a**3+6930*a**5-36*a**7)/(2027025-945945*a*a+51975*a**4-630*a**6+a**8)
    return rezultat 

def T_9(a):
    rezultat=(34459425*a-4729725*a**3+135135*a**5-990*a**7+a**9)/(34459425-16216200*a*a+945945*a**4-13860*a**6+45*a**8)
    return rezultat

def compute_T4():

    err = 0   

    for _ in range(10000):

        random_angle = random.uniform(-math.pi/2, math.pi/2)

        angle_t_value = T_4(random_angle)
        angle_exact_value = math.tan(random_angle)

        err1 = angle_t_value - angle_exact_value
        err1 = abs(err1)
        err = err + err1

    err = err / 10000
    global err_list
    err_list.append(err)
    

def compute_T5():

    err = 0   

    for _ in range(10000):

        random_angle = random.uniform(-math.pi/2, math.pi/2)

        angle_t_value = T_5(random_angle)
        angle_exact_value = math.tan(random_angle)

        err1 = angle_t_value - angle_exact_value
        err1 = abs(err1)
        err = err + err1

    err = err / 10000
    global err_list
    err_list.append(err)

def compute_T6():

    err = 0   

    for _ in range(10000):

        random_angle = random.uniform(-math.pi/2, math.pi/2)

        angle_t_value = T_6(random_angle)
        angle_exact_value = math.tan(random_angle)

        err1 = angle_t_value - angle_exact_value
        err1 = abs(err1)
        err = err + err1

    err = err / 10000
    global err_list
    err_list.append(err)

def compute_T7():

    err = 0   

    for _ in range(10000):

        random_angle = random.uniform(-math.pi/2, math.pi/2)

        angle_t_value = T_7(random_angle)
        angle_exact_value = math.tan(random_angle)

        err1 = angle_t_value - angle_exact_value
        err1 = abs(err1)
        err = err + err1

    err = err / 10000
    global err_list
    err_list.append(err)


def compute_T8():

    err = 0   

    for _ in range(10000):

        random_angle = random.uniform(-math.pi/2, math.pi/2)

        angle_t_value = T_8(random_angle)
        angle_exact_value = math.tan(random_angle)

        err1 = angle_t_value - angle_exact_value
        err1 = abs(err1)
        err = err + err1

    err = err / 10000
    global err_list
    err_list.append(err)

    

def compute_T9():

    err = 0   

    for _ in range(10000):

        random_angle = random.uniform(-math.pi/2, math.pi/2)

        angle_t_value = T_9(random_angle)
        angle_exact_value = math.tan(random_angle)

        err1 = angle_t_value - angle_exact_value
        err1 = abs(err1)
        err = err + err1

    err = err / 10000
    global err_list
    err_list.append(err)

compute_T4()
compute_T5()
compute_T6()
compute_T7()
compute_T8()
compute_T9()

err_list_with_positions = list(enumerate(err_list))

sorted_err_list = sorted(err_list_with_positions, key=lambda x: x[1])


position_mapping = {
    0: "T4",
    1: "T5",
    2: "T6",
    3: "T7",
    4: "T8",
    5: "T9"
}


smallest_values = sorted_err_list[:3]


for value, (position, error) in enumerate(smallest_values, start=1):
    mapped_position = position_mapping[position]
    print(f"Cea mai mica eraore {value}: Valoare: {error}, Functie: {mapped_position}")