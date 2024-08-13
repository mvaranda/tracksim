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

gSegmentClickID = 0

################# Railway Classes #################

SEG_POS_END = 1
SEG_POS_START = 0


########### Cammnads to UI ############
CMD_TRAIN_PRESENT = "CMD_TRAIN_PRESENT"
CMD_TRAIN_UNPRESENT = "CMD_TRAIN_UNPRESENT"
CMD_LIGHT_GREEN = "CMD_LIGHT_GREEN"
CMD_LIGHT_RED = "CMD_LIGHT_RED"
CMD_MESSAGE_DONE = "CMD_MESSAGE_DONE"
CMD_SEGMENT_RED = "CMD_SEGMENT_RED"

ICON_NONE = 0
ICON_INFO = 1
ICON_WARNING = 2
ICON_CRITICAL = 3
ICON_QUESTION = 4

#################################
#
#            T R A I N
#
#################################
class Train:
    """! The Train class.
    Provides all information passed by the UI. Simulator may add more variable if needed.
    """

    def __init__(
        self,
        ## Object identifier shared. UI also share the same ID.
        sim_id,
        train_number,
        speed,  # Num ticks to run over a segment
        enabled,
        reverse,
        start_time,  # in ticks
        route,
    ):
        ## Object identifier shared. UI also share the same ID.
        self.sim_id = sim_id
        ## Train number. UI uses this to render the train label.
        self.train_number = train_number
        self.speed = speed  # Num ticks to run over a segment
        self.enabled = enabled
        self.reverse = reverse
        self.start_time = start_time  # in ticks
        self.route = route
        self.location = route[0]  # segment sim_id of the first segment


#################################
#
#          S E G M E N T
#
#################################
class Segment:
    """Describes a Segment object.

    Segment details...
    """

    def __init__(
        self,
        sim_id,
        startTrackPoint_id,
        endTrackPoint_id,
        startLightState,
        endLightState,
    ):
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
        sim.cmd_to_ui(CMD_LIGHT_RED + " " + str(self.sim_id) + " " + str(seg_pos))

    def set_light_green(self, seg_pos):
        sim.cmd_to_ui(CMD_LIGHT_GREEN + " " + str(self.sim_id) + " " + str(seg_pos))

    def set_segment_red(self, yes_1_no_0):
        sim.cmd_to_ui(CMD_SEGMENT_RED + " " + str(self.sim_id) + " " + str(yes_1_no_0))


#################################
#
#       T R A C K P O I N T
#
#################################
class TrackPoint:
    """Describes a TrackPoint object.

    TrackPoint details...
    """

    def __init__(self, sim_id, segments):
        self.sim_id = sim_id
        self.segments = segments


####################################
#
#  H E L P E R   F U N C T I O N S
#
####################################
## Helper functions
def finish_ok(msg):
    sim.cmd_to_ui(CMD_MESSAGE_DONE + " " + str(ICON_INFO) + " " + msg)


def finish_error(msg):
    sim.cmd_to_ui(CMD_MESSAGE_DONE + " " + str(ICON_CRITICAL) + " " + msg)
