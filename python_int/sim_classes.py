

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

################# Railway Classes #################

TRAIN_STATE__INITIAL = 0

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

class TrackPoint:
  """Describes a TrackPoint object.

  TrackPoint details...
  """
  def __init__( self, 
                sim_id,
                segments):
    self.sim_id = sim_id
    self.segments = segments
