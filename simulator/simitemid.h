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

#pragma once

class SimItemID
{
public:
    SimItemID();
    SimItemID(int sim_id);
    int GetSimItemID();
protected:
    int id;
    static int s_id;
};
