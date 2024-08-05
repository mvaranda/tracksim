
###########################################################
#                                                         #
#               Part of the Tracks Simulator              #
#                                                         #
#  Copyright: Marcelo Varanda                             #
#  License: GPL3                                          #
#                                                         #
###########################################################

import sim


print(">>>>>>>>>>>>>>> Number of arguments", sim.numargs())

GLOBAL_VAR = "*********** This is my global var **************"

gItems = []
gSegments = []

def sim_init():
    print("\n******* Hello from simulator.py ********\n")

def print_global():
    print(GLOBAL_VAR)

def add_item(item_dic):
    global gItems
    print("receive an item dic, gItems now is like:");
    print(gItems)
    gItems.append(item_dic)
    return len(gItems)

def add_segment(seg_dic):
    global gSegments
    print("receive an segment dic, gSegments now is like:");
    print(gSegments)
    gSegments.append(seg_dic)
    return len(gSegments)