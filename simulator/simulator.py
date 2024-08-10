
###########################################################
#                                                         #
#               Part of the Tracks Simulator              #
#                                                         #
#  Copyright: Marcelo Varanda                             #
#  License: GPL3                                          #
#                                                         #
###########################################################

import traceback

if __name__ != "__main__":
    import sim
    print(">>>>>>>>>>>>>>> Number of arguments", sim.numargs())

import sim_store
import json


GLOBAL_VAR = "*********** This is my global var **************"

gItems = []
gSegments = []
gTexts = []
gTrains = []

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

def add_text(txt_dic):
    global gTexts
    print("receive an segment dic, gTexts now is like:")
    gTexts.append(txt_dic)
    print(gTexts)
    return len(gTexts)

def add_train(train):
    global gTrains
    print("receive an segment dic, gTrains now is like:")
    gTrains.append(train)
    print(gTrains)
    return len(gTrains)

def save(filename):
    return sim_store.store(filename, gItems, gSegments, gTexts, gTrains)

# def load(filename):
#     d = sim_store.load(filename)
#     if d == None:
#         return 0
#     print("dic:")
#     print(d)

# def _sim_create_item(p1): #,p2,p3,p4,p5,p6,p7):
#    print("fake _sim_create_item, id: " + str(p1))

def load(filename):
  try:
    f = open(filename)
    data = f.read()
    dic = json.loads(data)
    # print("********** load dic: ************")
    # print(dic)

    if "items" in dic:
      for i in dic["items"]:
        print("********** calling sim.create_item for item: ************")
        print(i)
        sim.create_item(i["sim_id"], 
                        i["type"],
                        i["pos_x"],
                        i["pos_y"],
                        i["color_r"],
                        i["color_g"],
                        i["color_b"] )
      
    if "segments" in dic:
      for seg in dic["segments"]:
        print("********** calling sim.create_segments for seg: ************")
        print(seg)
        sim.create_segment(seg["sim_id"], 
                        seg["type"],
                        seg["pos_x"],
                        seg["pos_y"],
                        seg["color_r"],
                        seg["color_g"],
                        seg["color_b"],
                        seg["startTrackPoint_id"],
                        seg["endTrackPoint_id"],
                        seg["startLightState"],
                        seg["endLightState"])

    if "texts" in dic:
      for seg in dic["texts"]:
        print("********** calling sim.create_texts for seg: ************")
        print(seg)
        sim.create_text(seg["text"],
                        seg["font_name"],
                        seg["size"],
                        seg["pos_x"],
                        seg["pos_y"],
                        seg["color_r"],
                        seg["color_g"],
                        seg["color_b"])

    if "trains" in dic:
      for train in dic["trains"]:
        print("********** calling sim.create_train for train: ************")
        print(train)
        sim.create_text(train["train_number"],
                        train["sim_id"],
                        train["pos_x"],
                        train["pos_y"],
                        train["speed"],
                        train["enabled"],
                        train["reverse"],
                        train["start_time"],
                        train["route_seg_ids"])
        
        
  except:
    print("Could not load " + filename)
    print(traceback.format_exc())
    return None

  return 1


## random tests
if __name__ == "__main__":
    load("test.rlw")
    # d = {'name': 'TrackPoint', 'type': 'TrackPoint', 'sim_id': 6, 'pos_x': 2275, 'pos_y': 2507, 'segment_id_0': 0, 'segment_id_1': 0, 'segment_id_2': 0, 'segment_id_3': 0}
    # add_item(d)
    # d2 = {'name': 'TrackPoint', 'type': 'TrackPoint', 'sim_id': 5, 'pos_x': 2539, 'pos_y': 2513, 'segment_id_0': 0, 'segment_id_1': 0, 'segment_id_2': 0, 'segment_id_3': 0}
    # add_item(d2)