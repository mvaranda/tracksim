
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
      "sim_id:": $SIM_ID,
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


# stored format example:
# {
#   "items": [
    # {
    #   "sim_id:": 10,
    #   "type": "trackpoint",
    #   "pos_x": 1234.5,
    #   "pos_y": 3456.8,
    #   "segments": [
    #     {
    #       "sim_id:": 12
    #     },
    #     {
    #       "sim_id:": 13
    #     }
    #   ]
    # },
#     {
#       "sim_id:": 11,
#       "type": "trackpoint",
#       "pos_x": 1234.5,
#       "pos_y": 3456.8
#     }
#   ],
#   "segments": [
#     {
#       "sim_id:": 12,
#       "type": "arrow",
#       "pos_x": 1234.5,
#       "pos_y": 3456.8,
#       "startTrackPoint_id": 1,
#       "endTrackPoint_id": 2,
#       "startLightState": 0,
#       "endLightState": 0
#     },
#     {
#       "sim_id:": 13,
#       "type": "arrow",
#       "pos_x": 1234.5,
#       "pos_y": 3456.8,
#       "startTrackPoint_id": 1,
#       "endTrackPoint_id": 2,
#       "startLightState": 0,
#       "endLightState": 0
#     }
#   ]
# }

def save(filename, items, segs):
  try:
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
        t = t.replace("$NAME", i["name"])

        t = t.replace("$TYPE", i["type"])
        t = t.replace("$POS_X", i["pos_x"])
        t = t.replace("$POS_Y", i["pos_y"])
        t = t.replace("$SIM_ID_0", i["segment_id_0"])
        t = t.replace("$SIM_ID_1", i["segment_id_0"])
        t = t.replace("$SIM_ID_2", i["segment_id_0"])
        t = t.replace("$SIM_ID_3", i["segment_id_0"])
        f.write(t)


if __name__ == "__main__":
  items = [{'name': 'TrackPoint', 'type': 'TrackPoint', 'sim_id': 4, 'pos_x': 2418, 'pos_y': 2346, \
            'segment_id_0': 7, 'segment_id_1': 8, 'segment_id_2': 0, 'segment_id_3': 0}, \
          {'name': 'TrackPoint', 'type': 'TrackPoint', 'sim_id': 4, 'pos_x': 2418, 'pos_y': 2346, \
           'segment_id_0': 7, 'segment_id_1': 8, 'segment_id_2': 0, 'segment_id_3': 0}]
  segs = [{'name': 'Segment', 'type': 'Segment', 'sim_id': 7, 'pos_x': 2275, 'pos_y': 2507, \
           'startTrackPoint_id': 6, 'endTrackPoint_id': 4, 'startLightState': 1, 'endLightState': 1}]
