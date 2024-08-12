

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

TRAIN_STATE__INITIAL = 0


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
    for s in self.segments:
      if s.sim_id == id:
        ret = s
        break
    return ret

## Helper functions
def obj_from_id(list, id):
  ret = None
  for s in list:
    if s.sim_id == id:
      ret = s
      break
  return ret

simRunner = None



def sim_start(trains, segments, tracks):
  global simRunner
  print("*************** Starting Simulator ***************")
  try:
    #
    # Create the simulator runner object
    #
    simRunner = SimRunner(trains, segments, tracks)

    ###### Search for intercessions #######
    print(simRunner.tracks)
    simRunner.intercessions = []       # add "intercession" variable to the object
    for track in simRunner.tracks:
      if len(track.segments) > 2:
        simRunner.intercessions.append(track)
        print("found intercession in TrackPoint " + str(track.sim_id) + ", segments:")
        print(track.segments)
        for seg_id in track.segments:
          seg = simRunner.segment_from_id(seg_id)
          ## for now only forward lights
          print("*** check endpoint ***")
          print(seg.endTrackPoint_id)
          print(track.sim_id)

          if seg.endTrackPoint_id == track.sim_id:
            seg.set_light_green(sim_classes.SEG_POS_END)
    
    # init trains
    for train in simRunner.trains:
      # place train in initial seg
      seg_id = train.route[0]
      seg = obj_from_id(simRunner.segments, seg_id)
      seg.set_train_present()
      train.state = TRAIN_STATE__INITIAL      # add "state" variable to the train object

  except:
     print("********************* exception ***********************")
     print(traceback.format_exc())

  return 1


def timer_tick():
  global simRunner

  for train in simRunner.trains:
    print("Train number: " + str(train.train_number))

  simRunner.tick_counter += 1

  if simRunner.tick_counter == 6:
    #sim_classes.finish_ok("OK Message. Bye Now MV !!!")
    sim_classes.finish_error("Error Message. Bye Now MV !!!")
    
  print("tick " + str(simRunner.tick_counter))



