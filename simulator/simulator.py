

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

def dummy_log(t):
   pass

log = print
#log = dummy_log    # uncomment to disable logs

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
                self.intercessions = []
                self.segs_with_light = []
                self.red_light_taken_segmets = []

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
  log("*************** Starting Simulator ***************")

  #
  # Create the simulator runner object
  #
  simRunner = SimRunner(trains, segments, tracks)

  ######## Initialize for intercessions and lights #######
  for track in simRunner.tracks:
    if len(track.segments) > 2:
      dic = {"trackpoint": track.sim_id, "taken_by_train_id": 0}
      simRunner.intercessions.append(track)
      segs = []
      for seg_id in track.segments:
        seg = simRunner.segment_from_id(seg_id)
        ## for now only forward lights

        if seg.endTrackPoint_id == track.sim_id:
          seg.set_light_green(sim_classes.SEG_POS_END)
          segs.append(seg.sim_id)
          simRunner.segs_with_light.append(seg.sim_id)
      dic["segs"] = segs
      simRunner.intercessions.append(dic)
  
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
    train.segs_in_red = [] # ex: [ {"route_seg" : 5, "seg_in_ref": 3}, {... same ...} ]

  ########## Initialize segments
  for seg in simRunner.segments:
    # add extra variable to segment object
    seg.red_flag = False

  return 1


def timer_tick():
  global simRunner

  ## run Train state machine
  for train in simRunner.trains:
    if train.state == TRAIN_STATE__INITIAL:
       train_state__initial(train)
    elif train.state == TRAIN_STATE__MOVING:
       train_state__moving(train)
    elif train.state == TRAIN_STATE__ARRIVED:
       train_state__arrived(train)
    else:
       log("unexpected state")

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
    
  if sim_classes.gSegmentClickID > 0:
    seg_id = sim_classes.gSegmentClickID
    sim_classes.gSegmentClickID = 0
    seg = obj_from_id(simRunner.segments, seg_id)
    if seg.red_flag == False:
      seg.red_flag = True
      seg.set_light_red(sim_classes.SEG_POS_END)
    else:
      seg.red_flag = False
      seg.set_light_green(sim_classes.SEG_POS_END)

  detect_seg = detect_collision()
  if detect_seg > 0:
     seg = obj_from_id(simRunner.segments, detect_seg)
     seg.set_segment_red(1)
     sim_classes.finish_error("Collision detected !!!")


def train_state__initial(train):
   global simRunner
   if simRunner.tick_counter > train.start_time:
      train.state = TRAIN_STATE__MOVING

def show_all_tracks():
  global simRunner
  log("------------------------------")
  log("show_all_tracks")
  for track in simRunner.tracks:
     log("track id = " + str(track.sim_id))
     log(track.segments)
  log("------------------------------")
     


def train_state__moving(train):
   global simRunner
   
   ## check if train has arrived
   if train.location == train.route[len(train.route) - 1]:
      if simRunner.tick_counter > (train.last_move_tick + train.speed):
        train.state = TRAIN_STATE__ARRIVED
      else:
         ## we need to wait to the tail be also in final seg
         return

      if train.tail_seg_id > 0:
        seg_id = train.tail_seg_id
        seg = obj_from_id(simRunner.segments, seg_id)
        seg.set_train_unpresent()
      return
   
   ## check if it is time to move the train to the next segment
   if simRunner.tick_counter > (train.last_move_tick + train.speed):

      # check if there is a signal taken for the next segment
      next_seg_id = train.route[train.route_idx] # + 1]
      next_seg_obj = obj_from_id(simRunner.segments, next_seg_id)
      
      if next_seg_obj.red_flag == True:  #next_seg_id in simRunner.red_light_taken_segmets:
         #log ("waiting on red light for segment " + str(next_seg_id))
         return
      
      # check if the next segment leads to an intercession
      if next_seg_id in simRunner.segs_with_light:
        inter_id = next_seg_obj.endTrackPoint_id
        track_obj = obj_from_id(simRunner.tracks, inter_id)
        track_segs_list = track_obj.segments.copy()
        track_segs_list.remove(next_seg_id)
        seg_obj_to_turn_red = None
        for other_seg_id in track_segs_list:
          other_seg_obj = obj_from_id(simRunner.segments, other_seg_id)
          if (other_seg_obj.endTrackPoint_id == inter_id):
              seg_obj_to_turn_red = other_seg_obj
              seg_obj_to_turn_red.set_light_red(sim_classes.SEG_POS_END)
              seg_obj_to_turn_red.red_flag = True
              # Add the other taken segment to segs_in_red
              d = {"route_seg": next_seg_id, "seg_in_red": seg_obj_to_turn_red.sim_id, }
              train.segs_in_red.append(d)


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

      ## remove red_flag for all free segs
      ## 
      i = 0
      remove_flag = False
      for d in train.segs_in_red:
        # d ex: {"route_seg" : 5, "seg_in_red": 3}
        if d["route_seg"] != train.location and d["route_seg"] != train.tail_seg_id:
          other_obj = obj_from_id(simRunner.segments, d["seg_in_red"])
          other_obj.red_flag = False
          other_obj.set_light_green(sim_classes.SEG_POS_END)
          remove_flag = True
          break
        i += 1
      if remove_flag == True:
         del(train.segs_in_red[i])
            

      #train.state == TRAIN_STATE__MOVING

def train_state__arrived(train):
   pass # nothing to do (placeholder)

def detect_collision():
  global simRunner
  for train in simRunner.trains:
    counter = 0
    loc = train.location
    tail = train.tail_seg_id
    if tail == 0:
       continue
    for other_train in simRunner.trains:
      if loc == other_train.location:
         counter += 1
      if tail == other_train.tail_seg_id:
         counter += 1
      if counter > 2:
        return loc 

  return 0
         