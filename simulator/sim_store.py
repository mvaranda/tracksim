
###########################################################
#                                                         #
#               Part of the Tracks Simulator              #
#                                                         #
#  Copyright: Marcelo Varanda                             #
#  License: GPL3                                          #
#                                                         #
###########################################################

DOC_START = """{
"""

DOC_END = """}
"""

ITEMS_START = """  "items": [
"""

ITEM = """    {
      "sim_id:": "$SIM_ID",
      "name:": "$NAME",
      "type": "$TYPE",
      "pos_x": $POS_X,
      "pos_y": $POS_Y,
      "segments": [
        {
          "sim_id:": $SIM_ID_0
        },
        {
          "sim_id:": $SIM_ID_1
        },
        {
          "sim_id:": $SIM_ID_2
        },
        {
          "sim_id:": $SIM_ID_3
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
      "sim_id:": "$SIM_ID",
      "name:": "$NAME",
      "type": "$TYPE",
      "pos_x": $POS_X,
      "pos_y": $POS_Y,
      "startTrackPoint_id": $START_TRACKPOINT_ID,
      "endTrackPoint_id": $END_TRACKPOINT_ID,
      "startLightState": $START_LIGHT_STATE,
      "endLightState": $END_LIGHT_STATE,
    }"""


# stored format example:

"""
{
  "items": [
    {
      "sim_id:": 10,
      "type": "trackpoint",
      "pos_x": 1234.5,
      "pos_y": 3456.8,
      "segments": [
        {
          "sim_id:": 12
        },
        {
          "sim_id:": 13
        }
      ]
    },
    {
      "sim_id:": 11,
      "type": "trackpoint",
      "pos_x": 1234.5,
      "pos_y": 3456.8
    }
  ],
  "segments": [
    {
      "sim_id:": 12,
      "type": "arrow",
      "pos_x": 1234.5,
      "pos_y": 3456.8,
      "startTrackPoint_id": 1,
      "endTrackPoint_id": 2,
      "startLightState": 0,
      "endLightState": 0
    },
    {
      "sim_id:": 13,
      "type": "arrow",
      "pos_x": 1234.5,
      "pos_y": 3456.8,
      "startTrackPoint_id": 1,
      "endTrackPoint_id": 2,
      "startLightState": 0,
      "endLightState": 0
    }
  ]
}
"""

def save(filename, items, segs):
  #try:
    f = open(filename, "w")
    f.write(DOC_START)

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
        t = t.replace("$SIM_ID_0", str(i["segment_id_0"]))
        t = t.replace("$SIM_ID_1", str(i["segment_id_1"]))
        t = t.replace("$SIM_ID_2", str(i["segment_id_2"]))
        t = t.replace("$SIM_ID_3", str(i["segment_id_3"]))
        f.write(t)
      f.write(ITEMS_END)

    if len(segs) > 0:
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
        t = t.replace("$START_TRACKPOINT_ID", str(i["startTrackPoint_id"]))
        t = t.replace("$END_TRACKPOINT_ID", str(i["endTrackPoint_id"]))
        t = t.replace("$START_LIGHT_STATE", str(i["startLightState"]))
        t = t.replace("$END_LIGHT_STATE", str(i["endLightState"]))
        f.write(t)
      f.write(SEGMENTS_END)
    f.write(DOC_END)
    f.close()
  # except:
  #   print("Could not save " + filename)
  #   return 0
  # all good
    return 1

if __name__ == "__main__":
  items = [{'name': 'TrackPoint', 'type': 'TrackPoint', 'sim_id': 4, 'pos_x': 2418, 'pos_y': 2346, \
            'segment_id_0': 7, 'segment_id_1': 8, 'segment_id_2': 0, 'segment_id_3': 0}, \
          {'name': 'TrackPoint', 'type': 'TrackPoint', 'sim_id': 4, 'pos_x': 2418, 'pos_y': 2346, \
           'segment_id_0': 7, 'segment_id_1': 8, 'segment_id_2': 0, 'segment_id_3': 0}]
  segs = [{'name': 'Segment', 'type': 'Segment', 'sim_id': 7, 'pos_x': 2275, 'pos_y': 2507, \
           'startTrackPoint_id': 6, 'endTrackPoint_id': 4, 'startLightState': 1, 'endLightState': 1}]
  if save("test.rlw", items, segs) != 0:
    print("All good")
  else:
    print("Error")

[{'name': 'TrackPoint', 'type': 'TrackPoint', 'sim_id': 4, 'pos_x': 2418, 'pos_y': 2346, 'segment_id_0': 7, 'segment_id_1': 8, 'segment_id_2': 0, 'segment_id_3': 0}, 
 {'name': 'TrackPoint', 'type': 'TrackPoint', 'sim_id': 4, 'pos_x': 2418, 'pos_y': 2346, 'segment_id_0': 7, 'segment_id_1': 8, 'segment_id_2': 0, 'segment_id_3': 0}]


[{'name': 'TrackPoint', 'type': 'TrackPoint', 'sim_id': 4, 'pos_x': 2418, 'pos_y': 2346, 'segment_id_0': 7, 'segment_id_1': 8, 'segment_id_2': 0, 'segment_id_3': 0}, 
 {'name': 'TrackPoint', 'type': 'TrackPoint', 'sim_id': 4, 'pos_x': 2418, 'pos_y': 2346, 'segment_id_0': 7, 'segment_id_1': 8, 'segment_id_2': 0, 'segment_id_3': 0}]