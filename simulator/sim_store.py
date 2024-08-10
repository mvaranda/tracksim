#!/usr/bin/python3
###########################################################
#                                                         #
#               Part of the Tracks Simulator              #
#                                                         #
#  Copyright: Marcelo Varanda                             #
#  License: GPL3                                          #
#                                                         #
###########################################################
import traceback

DOC_START = """{
"""

DOC_END = """}
"""

ITEMS_START = """  "items": [
"""

ITEM = """    {
      "sim_id": $SIM_ID,
      "name": "$NAME",
      "type": "$TYPE",
      "pos_x": $POS_X,
      "pos_y": $POS_Y,
      "color_r": $COLOR_R,
      "color_g": $COLOR_G,
      "color_b": $COLOR_B,
      "segments": [
        {
          "sim_id": $SEG_SIM_ID_0
        },
        {
          "sim_id": $SEG_SIM_ID_1
        },
        {
          "sim_id": $SEG_SIM_ID_2
        },
        {
          "sim_id": $SEG_SIM_ID_3
        }
      ]
    }"""

ITEMS_END = """
  ]
"""

SEGMENTS_START = """  "segments": [
"""
SEGMENTS_END = """
  ]
"""

SEGMENTS = """    {
      "sim_id": $SIM_ID,
      "name": "$NAME",
      "type": "$TYPE",
      "pos_x": $POS_X,
      "pos_y": $POS_Y,
      "color_r": $COLOR_R,
      "color_g": $COLOR_G,
      "color_b": $COLOR_B,
      "startTrackPoint_id": $START_TRACKPOINT_ID,
      "endTrackPoint_id": $END_TRACKPOINT_ID,
      "startLightState": $START_LIGHT_STATE,
      "endLightState": $END_LIGHT_STATE
    }"""

###
TEXT_START = """  "texts": [
"""
TEXT_END = """
  ]
"""

TEXTS = """    {
      "text": "$TEXT",
      "font_name": "$FONT_NAME",
      "size": $SIZE,
      "pos_x": $POS_X,
      "pos_y": $POS_Y,
      "color_r": $COLOR_R,
      "color_g": $COLOR_G,
      "color_b": $COLOR_B
    }"""

###
TRAINS_START = """  "trains": [
"""
TRAIN_END = """
      ]
    }"""

TRAINS_END = """
  ]\n"""

TRAIN = """    {
      "sim_id": $SIM_ID,
      "train_number": "$TRAIN_NUMBER",
      "pos_x": $POS_X,
      "pos_y": $POS_Y,
      "speed": $SPEED,
      "enabled": $ENABLED,
      "reverse": $REVERSE,
      "start_time": $START_TIME,
      "route_seg_ids": [
        """

TRAIN_TEST = """    {
      "start_time": $START_TIME,
      "route_seg_ids": [
        """

ROUTE_END = """
       ]
    }"""


# stored format example:

"""
{
  "items": [
    {
      "sim_id": "6",
      "name": "TrackPoint",
      "type": "TrackPoint",
      "pos_x": 2275,
      "pos_y": 2507,
      "color_r": 255,
      "color_g": 0,
      "color_b": 0,
      "segments": [
        {
          "sim_id": 7
        },
        {
          "sim_id": 0
        },
        {
          "sim_id": 0
        },
        {
          "sim_id": 0
        }
      ]
    },
    {
      "sim_id": "5",
      "name": "TrackPoint",
      "type": "TrackPoint",
      "pos_x": 2539,
      "pos_y": 2513,
      "color_r": 0,
      "color_g": 255,
      "color_b": 0,
      "segments": [
        {
          "sim_id": 8
        },
        {
          "sim_id": 0
        },
        {
          "sim_id": 0
        },
        {
          "sim_id": 0
        }
      ]
    },
    {
      "sim_id": "4",
      "name": "TrackPoint",
      "type": "TrackPoint",
      "pos_x": 2418,
      "pos_y": 2346,
      "color_r": 0,
      "color_g": 0,
      "color_b": 255,
      "segments": [
        {
          "sim_id": 7
        },
        {
          "sim_id": 8
        },
        {
          "sim_id": 0
        },
        {
          "sim_id": 0
        }
      ]
    }
  ],
  "segments": [
    {
      "sim_id": "8",
      "name": "Segment",
      "type": "Segment",
      "pos_x": 2418,
      "pos_y": 2346,
      "color_r": 0,
      "color_g": 0,
      "color_b": 0,
      "startTrackPoint_id": 4,
      "endTrackPoint_id": 5,
      "startLightState": 1,
      "endLightState": 1
    },
    {
      "sim_id": "7",
      "name": "Segment",
      "type": "Segment",
      "pos_x": 2275,
      "pos_y": 2507,
      "color_r": 0,
      "color_g": 0,
      "color_b": 0,
      "startTrackPoint_id": 6,
      "endTrackPoint_id": 4,
      "startLightState": 1,
      "endLightState": 1
    }
  ]
}
"""

def store(filename, items, segs, texts, trains):
  try:
    f = open(filename, "w")
    f.write(DOC_START)

    ########### Add TrackPoints ##########
    if len(items) > 0:
      f.write(ITEMS_START)
      is_first = True
      for i in items:
        if is_first == False:
          f.write(",\n")
        is_first = False
        t = ITEM
        t = t.replace("$SIM_ID", str(i["sim_id"]))
        t = t.replace("$NAME", i["name"])
        t = t.replace("$TYPE", i["type"])
        t = t.replace("$POS_X", str(i["pos_x"]))
        t = t.replace("$POS_Y", str(i["pos_y"]))
        t = t.replace("$COLOR_R", str(i["color_r"]))
        t = t.replace("$COLOR_G", str(i["color_g"]))
        t = t.replace("$COLOR_B", str(i["color_b"]))
        t = t.replace("$SEG_SIM_ID_0", str(i["segment_id_0"]))
        t = t.replace("$SEG_SIM_ID_1", str(i["segment_id_1"]))
        t = t.replace("$SEG_SIM_ID_2", str(i["segment_id_2"]))
        t = t.replace("$SEG_SIM_ID_3", str(i["segment_id_3"]))
        f.write(t)
      f.write(ITEMS_END)

    ########### Add Segments ##########
    if len(segs) > 0:
      if len(items) > 0:
        f.write(",\n")
      f.write(SEGMENTS_START)
      is_first = True
      for i in segs:
        if is_first == False:
          f.write(",\n")
        is_first = False
        t = SEGMENTS
        t = t.replace("$SIM_ID", str(i["sim_id"]))
        t = t.replace("$NAME", i["name"])
        t = t.replace("$TYPE", i["type"])
        t = t.replace("$POS_X", str(i["pos_x"]))
        t = t.replace("$POS_Y", str(i["pos_y"]))
        t = t.replace("$COLOR_R", str(i["color_r"]))
        t = t.replace("$COLOR_G", str(i["color_g"]))
        t = t.replace("$COLOR_B", str(i["color_b"]))
        t = t.replace("$START_TRACKPOINT_ID", str(i["startTrackPoint_id"]))
        t = t.replace("$END_TRACKPOINT_ID", str(i["endTrackPoint_id"]))
        t = t.replace("$START_LIGHT_STATE", str(i["startLightState"]))
        t = t.replace("$END_LIGHT_STATE", str(i["endLightState"]))
        f.write(t)
      f.write(SEGMENTS_END)

    # print("texts:")
    # print(texts)

    ########### Add texts ##########
    if len(texts) > 0:
      if len(items) > 0 or len(segs) > 0:
        f.write(",\n")
      f.write(TEXT_START)
      is_first = True
      for i in texts:
        if is_first == False:
          f.write(",\n")
        is_first = False
        t = TEXTS
        t = t.replace("$TEXT", i["text"])
        t = t.replace("$FONT_NAME", i["font_name"])
        t = t.replace("$SIZE", str(i["size"]))
        t = t.replace("$POS_X", str(i["pos_x"]))
        t = t.replace("$POS_Y", str(i["pos_y"]))
        t = t.replace("$COLOR_R", str(i["color_r"]))
        t = t.replace("$COLOR_G", str(i["color_g"]))
        t = t.replace("$COLOR_B", str(i["color_b"]))
        f.write(t)
      f.write(TEXT_END)
    else:
      print("no texts")

    ########### Add trains ##########
    if len(trains) > 0:
      if len(items) > 0 or len(segs) > 0 or len(texts) > 0:
        f.write(",\n")
      f.write(TRAINS_START)
      is_first = True
      for i in trains:
        if is_first == False:
          f.write(",\n")
        is_first = False
        t = TRAIN
        t = t.replace("$TRAIN_NUMBER", str(i["train_number"]))
        t = t.replace("$SIM_ID", str(i["sim_id"]))
        t = t.replace("$POS_X", str(i["pos_x"]))
        t = t.replace("$POS_Y", str(i["pos_y"]))

        t = t.replace("$SPEED", str(i["speed"]))
        t = t.replace("$ENABLED", str(i["enabled"]))
        t = t.replace("$REVERSE", str(i["reverse"]))
        t = t.replace("$START_TIME", str(i["start_time"]))

        f.write(t)

        # array of route_seg_ids:
        first_seg = True
        for seg in i["route"]:
          if first_seg == False:
            f.write(", " )
          f.write(str(seg))
          first_seg = False


        f.write(TRAIN_END)
      f.write(TRAINS_END) # + "\n")

    ########### end document ##########
    f.write(DOC_END)
    f.close()

    ## Test only:
    print("JSON File:\n")
    f = open(filename)
    d = f.read()
    f.close()
    print(d)

  except:
    print("Could not save " + filename)
    print(traceback.format_exc())
    return 0
  #all good
  return 1


############### tests ###############
def test_store():
  items = [{'name': 'TrackPoint', 'type': 'TrackPoint', 'sim_id': 6, 'pos_x': 2275, 'pos_y': 2507, 
            'color_r': 255, 'color_g': 0, 'color_b': 0, 'segment_id_0': 7, 'segment_id_1': 0, 
            'segment_id_2': 0, 'segment_id_3': 0}, {'name': 'TrackPoint', 'type': 'TrackPoint', 'sim_id': 5, 
            'pos_x': 2539, 'pos_y': 2513, 'color_r': 0, 'color_g': 255, 'color_b': 0, 'segment_id_0': 8, 
            'segment_id_1': 0, 'segment_id_2': 0, 'segment_id_3': 0}, {'name': 'TrackPoint', 'type': 'TrackPoint', 
            'sim_id': 4, 'pos_x': 2418, 'pos_y': 2346, 'color_r': 0, 'color_g': 0, 'color_b': 255, 'segment_id_0': 7, 
            'segment_id_1': 8, 'segment_id_2': 0, 'segment_id_3': 0}]
  
  segs = [{'name': 'Segment', 'type': 'Segment', 'sim_id': 8, 'pos_x': 2418, 'pos_y': 2346, 
           'color_r': 0, 'color_g': 0, 'color_b': 0, 'startTrackPoint_id': 4, 'endTrackPoint_id': 5, 
           'startLightState': 1, 'endLightState': 1}, 
           {'name': 'Segment', 'type': 'Segment', 'sim_id': 7, 'pos_x': 2275, 'pos_y': 2507, 
            'color_r': 0, 'color_g': 0, 'color_b': 0, 'startTrackPoint_id': 6, 'endTrackPoint_id': 4, 
            'startLightState': 1, 'endLightState': 1}]
  if store("test.rlw", items, segs) != 0:
    print("All good")
  else:
    print("Error")

def test_store_train():
  trains = [{'train_number': 2, 'sim_id': 14, 'pos_x': 2191, 'pos_y': 2651, 'speed': 5, 'start_time': 5, 'enabled': 1, 'reverse': 0, 'route': [12, 11, 10]},
             {'train_number': 1, 'sim_id': 13, 'pos_x': 2170, 'pos_y': 2525, 'speed': 5, 'start_time': 5, 'enabled': 1, 'reverse': 0, 'route': [9, 8]}]

  items = []
  segs = [] 
  texts = []
  print("test_store_train")
  store("t.json", items, segs, texts, trains)


if __name__ == "__main__":
  #test_store()
  test_store_train()


