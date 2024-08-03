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
    int GetSimItemID();
protected:
    int id;
    static int s_id;
};
