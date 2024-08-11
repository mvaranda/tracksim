

###########################################################
#                                                         #
#               Part of the Tracks Simulator              #
#                                                         #
#  Copyright: Marcelo Varanda                             #
#  License: GPL3                                          #
#                                                         #
###########################################################

"""@package sim_classes
Documentation for this sim_classes.
 
Provide the pre-defined classes that the TrackSim feeds the simulator.
"""

import traceback
import sim

################# Railway Classes #################

SEG_POS_END = "end"
SEG_POS_START = "start"


########### Cammnads to UI ############
CMD_TRAIN_PRESENT = "CMD_TRAIN_PRESENT"
CMD_TRAIN_UNPRESENT = "CMD_TRAIN_UNPRESENT"
CMD_LIGHT_GREEN = "CMD_LIGHT_GREEN"
CMD_LIGHT_RED = "CMD_LIGHT_RED"
CMD_MESSAGE_DONE = "CMD_MESSAGE_DONE"

ICON_NONE = 0
ICON_INFO = 1
ICON_WARNING = 2
ICON_CRITICAL = 3
ICON_QUESTION = 4


class Train:
  """Describes a Train object.

  Train details...
  """
  def __init__( self, 
                sim_id,
                train_number,
                speed,      # Num ticks to run over a segment
                enabled,
                reverse,
                start_time,  # in ticks
                route):
    self.sim_id = sim_id
    self.train_number = train_number
    self.speed = speed      # Num ticks to run over a segment
    self.enabled = enabled
    self.reverse = reverse
    self.start_time = start_time  # in ticks
    self.route = route


class Segment:
  """Describes a Segment object.

  Segment details...
  """
  def __init__( self, 
                sim_id,
                startTrackPoint_id,
                endTrackPoint_id,
                startLightState,
                endLightState):
    self.sim_id = sim_id
    self.startTrackPoint_id = startTrackPoint_id
    self.endTrackPoint_id = endTrackPoint_id
    self.startLightState = startLightState
    self.endLightState = endLightState

  def set_train_present(self):
    sim.cmd_to_ui(CMD_TRAIN_PRESENT + " " + str(self.sim_id))

  def set_train_unpresent(self):
    sim.cmd_to_ui(CMD_TRAIN_UNPRESENT + " " + str(self.sim_id))

  def set_light_red(self, seg_pos):
    sim.cmd_to_ui(CMD_LIGHT_RED + " " + str(self.sim_id) + " " + seg_pos)

  def set_light_green(self, seg_pos):
    print("--green --")
    sim.cmd_to_ui(CMD_LIGHT_GREEN + " " + str(self.sim_id) + " " + seg_pos)



class TrackPoint:
  """Describes a TrackPoint object.

  TrackPoint details...
  """
  def __init__( self, 
                sim_id,
                segments):
    self.sim_id = sim_id
    self.segments = segments

## Helper functions
def finish_ok(msg):
  sim.cmd_to_ui(CMD_MESSAGE_DONE  + " " + str(ICON_INFO) + " " + msg)

def finish_error(msg):
  sim.cmd_to_ui(CMD_MESSAGE_DONE  + " " + str(ICON_CRITICAL) + " " + msg)