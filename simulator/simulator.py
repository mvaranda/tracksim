

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

class SimRunner:
  """Describes a Train object.

  Train details...
  """
  def __init__(self, trains, segments, tracks):
                self.trains = trains
                self.segments = segments
                self.tracks = tracks
                self.tick_counter = 0

  def segment_from_id(self, id):
    ret = None
    for s in segments:
      if s.sim_id == id:
        ret = s
        break
    return ret


simRunner = None



def sim_start(trains, segments, tracks):
    global simRunner
    print("*************** Starting Simulator ***************")
    #
    # Create the simulator runner object
    #
    simRunner = SimRunner(trains, segments, tracks)


def timer_tick():
    global simRunner

    for train in simRunner.trains:
      print("Train number: " + str(train.train_number))

    simRunner.tick_counter += 1
    print("tick " + str(simRunner.tick_counter))



