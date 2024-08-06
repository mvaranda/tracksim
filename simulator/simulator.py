
###########################################################
#                                                         #
#               Part of the Tracks Simulator              #
#                                                         #
#  Copyright: Marcelo Varanda                             #
#  License: GPL3                                          #
#                                                         #
###########################################################

if __name__ != "__main__":
    import sim
    print(">>>>>>>>>>>>>>> Number of arguments", sim.numargs())

import sim_store
import json


GLOBAL_VAR = "*********** This is my global var **************"

gItems = []
gSegments = []

def sim_init():
    print("\n******* Hello from simulator.py ********\n")

def print_global():
    print(GLOBAL_VAR)

def add_item(item_dic):
    global gItems
    print("receive an item dic:")
    print(item_dic)
    gItems.append(item_dic)
    print("gItems now is like:")
    print(gItems)
    return len(gItems)

def add_segment(seg_dic):
    global gSegments
    print("receive an segment dic, gSegments now is like:")
    gSegments.append(seg_dic)
    print(gSegments)
    return len(gSegments)

def save(filename):
    return sim_store.save(filename, gItems, gSegments)

# def load(filename):
#     d = sim_store.load(filename)
#     if d == None:
#         return 0
#     print("dic:")
#     print(d)

def load(filename):
  try:
    f = open(filename)
    data = f.read()
    dic = json.loads(data)
    print(j)
  except:
    print("could not load " + filename)
    return None
  for i in dic["items"]:
    print(i)
    sim.create_item(i["sim_id"], 
                    i["type"],
                    i["pos_x"],
                    i["pos_y"],
                    i["color_r"],
                    i["color_g"],
                    i["color_b"] )

  return 1

## random tests
if __name__ == "__main__":
    load("test.rlw")
    # d = {'name': 'TrackPoint', 'type': 'TrackPoint', 'sim_id': 6, 'pos_x': 2275, 'pos_y': 2507, 'segment_id_0': 0, 'segment_id_1': 0, 'segment_id_2': 0, 'segment_id_3': 0}
    # add_item(d)
    # d2 = {'name': 'TrackPoint', 'type': 'TrackPoint', 'sim_id': 5, 'pos_x': 2539, 'pos_y': 2513, 'segment_id_0': 0, 'segment_id_1': 0, 'segment_id_2': 0, 'segment_id_3': 0}
    # add_item(d2)