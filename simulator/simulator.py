

###########################################################
#                                                         #
#               Part of the Tracks Simulator              #
#                                                         #
#  Copyright: Marcelo Varanda                             #
#  License: GPL3                                          #
#                                                         #
###########################################################

import traceback
import sim, sim_classes

"""
  For class definitions read "sim_classes.py" file.
"""
################# Railway Classes #################

g_trains = None
g_segments = None
g_tracks = None

def segment_from_id(id):
  ret = None
  for s in g_segments:
    if s.sim_id == id:
      ret = s
      break
  return ret

def sim_start(trains, segments, tracks):
  global g_trains
  global g_segments
  global g_tracks
  g_trains = trains
  g_segments = segments
  g_tracks = tracks

  print("hello from sim, trains:")
  for train in trains:
    print("Train number: " + str(train.train_number))
  #sim.cmd_to_ui("This is a command to UI")
  segment_id = trains[0].route[0]
  seg_obj = segment_from_id(segment_id)

  seg_obj.set_train_on()
  seg_obj.set_train_off()
  seg_obj.set_light_red(sim_classes.SEG_POS_END)


