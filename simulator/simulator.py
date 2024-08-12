

###########################################################
#                                                         #
#               Part of the Tracks Simulator              #
#                                                         #
#  Copyright: Marcelo Varanda                             #
#  License: GPL3                                          #
#                                                         #
###########################################################


import sim_classes

"""
  For class definitions read "sim_classes.py" file.
"""

TRAIN_STATE__INITIAL = 0
TRAIN_STATE__MOVING = 1
TRAIN_STATE__ARRIVED = 2


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

  #
  # Create the simulator runner object
  #
  simRunner = SimRunner(trains, segments, tracks)

  ######## Initialize for intercessions and lights #######
  print(simRunner.tracks)
  simRunner.intercessions = []       # add "intercession" variable to the object
  for track in simRunner.tracks:
    if len(track.segments) > 2:
      dic = {"trackpoint": track.sim_id}
      simRunner.intercessions.append(track)
      print("found intercession in TrackPoint " + str(track.sim_id) + ", segments:")
      print(track.segments)
      segs = []
      for seg_id in track.segments:
        seg = simRunner.segment_from_id(seg_id)
        ## for now only forward lights

        if seg.endTrackPoint_id == track.sim_id:
          seg.set_light_green(sim_classes.SEG_POS_END)
          segs.append(seg.sim_id)
      dic["segs"] = segs
      simRunner.intercessions.append(dic)
  print(simRunner.intercessions)
  
  ########## Initialize trains
  for train in simRunner.trains:
    # place train in initial seg
    seg_id = train.location
    seg = obj_from_id(simRunner.segments, seg_id)
    seg.set_train_present()

    # add extra variable to train object
    train.state = TRAIN_STATE__INITIAL
    train.last_move_tick = 0
    train.route_idx = 0
    train.tail_seg_id = 0

  return 1


def timer_tick():
  global simRunner

  ## run Train state machine
  for train in simRunner.trains:
    print("Train number: " + str(train.train_number))
    if train.state == TRAIN_STATE__INITIAL:
       train_state__initial(train)
    elif train.state == TRAIN_STATE__MOVING:
       train_state__moving(train)
    elif train.state == TRAIN_STATE__ARRIVED:
       train_state__arrived(train)
    else:
       print("unexpected state")

  ## detect arrivals
  finished = True
  for train in simRunner.trains:
     if train.state != TRAIN_STATE__ARRIVED:
        finished = False
        break
  if finished == True:
     sim_classes.finish_ok("Simulation ended fine !!!")
     return
     

  simRunner.tick_counter += 1



  if simRunner.tick_counter == 20:
    sim_classes.finish_ok("Simulation ended fine !!!")
    
  print("tick " + str(simRunner.tick_counter))


def train_state__initial(train):
   global simRunner
   print("**** state_initial ****")
   if simRunner.tick_counter > train.start_time:
      train.state = TRAIN_STATE__MOVING

def train_state__moving(train):
   global simRunner
   print("**** state moving ****")
   
   ## check if train has arrived
   if train.location == train.route[len(train.route) - 1]:
      train.state = TRAIN_STATE__ARRIVED

      if train.tail_seg_id > 0:
        seg_id = train.tail_seg_id
        seg = obj_from_id(simRunner.segments, seg_id)
        seg.set_train_unpresent()
      return
   
   ## check if it is time to move the train to the next segment
   if simRunner.tick_counter > (train.last_move_tick + train.speed):
      # move train
      # seg_id = train.location
      # seg = obj_from_id(simRunner.segments, seg_id)
      # seg.set_train_unpresent()
      train.route_idx += 1
      seg_id = train.route[train.route_idx]
      train.location = seg_id
      seg = obj_from_id(simRunner.segments, seg_id)
      seg.set_train_present()

      # remove tail
      if train.route_idx > 1:
        seg_id = train.route[train.route_idx - 2]
        seg = obj_from_id(simRunner.segments, seg_id)
        seg.set_train_unpresent()
        train.tail_seg_id = train.route[train.route_idx - 1]

      train.last_move_tick = simRunner.tick_counter


      #train.state == TRAIN_STATE__MOVING

def train_state__arrived(train):
   pass # nothing to do (placeholder)