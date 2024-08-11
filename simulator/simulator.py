

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


def sim_start(trains, segments, tracks):
  print("hello from sim, trains:")
  for train in trains:
    print("Train number: " + str(train.train_number))
