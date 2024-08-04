
GLOBAL_VAR = "*********** This is my global var **************"

gItems = []

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
    global gItems
    print("receive an item dic, gItens now is like:");
    print(gItems)
    gItems.append(item_dic)
    return len(gItems)