

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

TRAIN_STATE__INITIAL = 0
SEG_POS_END = "end"
SEG_POS_START = "start"

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
    self.state = TRAIN_STATE__INITIAL


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

  def set_train_on(self):
    sim.cmd_to_ui("set_train_on " + str(self.sim_id))

  def set_train_off(self):
    sim.cmd_to_ui("set_train_off " + str(self.sim_id))

  def set_light_red(self, seg_pos):
    sim.cmd_to_ui("set_light_red " + str(self.sim_id) + " " + seg_pos)

  def set_light_green(self, seg_pos):
    sim.cmd_to_ui("set_light_green " + str(self.sim_id) + " " + seg_pos)

class TrackPoint:
  """Describes a TrackPoint object.

  TrackPoint details...
  """
  def __init__( self, 
                sim_id,
                segments):
    self.sim_id = sim_id
    self.segments = segments
