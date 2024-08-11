
###########################################################
#                                                         #
#               Part of the Tracks Simulator              #
#                                                         #
#  Copyright: Marcelo Varanda                             #
#  License: GPL3                                          #
#                                                         #
###########################################################

import traceback
import sim_classes
import simulator

if __name__ != "__main__":
    import sim

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
    gItems.append(item_dic)
    return len(gItems)

def add_segment(seg_dic):
    global gSegments
    gSegments.append(seg_dic)
    return len(gSegments)

def add_text(txt_dic):
    global gTexts
    gTexts.append(txt_dic)
    return len(gTexts)

def add_train(train):
    global gTrains
    gTrains.append(train)
    return len(gTrains)

def save(filename):
    return sim_store.store(filename, gItems, gSegments, gTexts, gTrains)

def start():
  trains = []
  segments = []
  tracks = []

  for t in gTrains:
    ## if train is disabled we ignore it
    if t["enabled"] == 0:
       continue
    ## sort route
    route = t["route"]
    if t["reverse"] != 0:
       route.sort(reverse=True)
    else:
       route.sort()
    train = sim_classes.Train(
              t["sim_id"],
              t["train_number"],
              t["speed"],      # Num ticks to run over a segment
              t["enabled"],
              t["reverse"],
              t["start_time"],  # in ticks
              route)
    trains.append(train)


  for t in gSegments:
    seg = sim_classes.Segment(
              t["sim_id"],
              t["startTrackPoint_id"],
              t["endTrackPoint_id"],      # Num ticks to run over a segment
              t["startLightState"],
              t["endLightState"])
    segments.append(seg)

  for t in gItems:
    print("TrackPoint:")
    print(t)
    segs = []
    if t["segment_id_0"] != 0:
       segs.append(t["segment_id_0"])
    if t["segment_id_1"] != 0:
       segs.append(t["segment_id_1"])
    if t["segment_id_2"] != 0:
       segs.append(t["segment_id_2"])
    if t["segment_id_3"] != 0:
       segs.append(t["segment_id_3"])

    track = sim_classes.TrackPoint(
              t["sim_id"],
              segs)
    tracks.append(track)


  simulator.sim_start(trains, segments, tracks)

def timer_tick():
   simulator.timer_tick()

def load(filename):
  try:
    f = open(filename)
    data = f.read()
    dic = json.loads(data)

    if "items" in dic:
      for i in dic["items"]:
        sim.create_item(i["sim_id"], 
                        i["type"],
                        i["pos_x"],
                        i["pos_y"],
                        i["color_r"],
                        i["color_g"],
                        i["color_b"] )
      
    if "segments" in dic:
      for seg in dic["segments"]:
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
        sim.create_train(train["train_number"],
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