
GLOBAL_VAR = "*********** This is my global var **************"

def sim_init():
    print("\n******* Hello from simulator.py ********\n")

def multiply(a,b):
    print("Will compute", a, "times", b)
    c = 0
    for i in range(0, a):
        c = c + b
    return c

def print_global():
    print(GLOBAL_VAR)

def add_item(item_dic):
    print("receive an item dic:");
    print(item_dic)
    return 100