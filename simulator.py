def sim_init():
    print("\n******* Hello from simulator.py ********\n")

def multiply(a,b):
    print("Will compute", a, "times", b)
    c = 0
    for i in range(0, a):
        c = c + b
    return c