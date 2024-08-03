/*
*************************************************************
*                                                           *
*              Part of the Tracks Simulator                 *
*                                                           *
* Copyright: Marcelo Varanda                                *
* License: GPL3                                             *
*                                                           *
************************************************************
*/

#include "simitemid.h"

int SimItemID::s_id = 0;

SimItemID::SimItemID() {
    id = s_id++;
}

int SimItemID::GetSimItemID() { return id; }

