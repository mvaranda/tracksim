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

#include "segment.h"

#include <iostream>

class Simulator // is a Singleton
{
protected:

    static Simulator* sim_instance;
    Simulator(){}

    std::string value_;

public:
    //  not be cloneable
    Simulator(Simulator &other) = delete;
    void operator=(const Simulator &) = delete;

    static Simulator *GetInstance();

    void AddSegment(  int id, 
                      float x = 0, 
                      float y = 0, 
                      bool has_end_light = false, 
                      bool has_start_light = false,
                      bool is_station = false);

};
