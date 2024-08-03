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

#include "simulator.h"
#include "log.h"
#include <iostream>


Simulator* Simulator::sim_instance = nullptr;;

/**
 * Static methods should be defined outside the class.
 */
Simulator *Simulator::GetInstance() //const std::string& value)
{
    /**
     * This is a safer way to create an instance. instance = new Simulator is
     * dangeruous in case two instance threads wants to access at the same time
     */
    if(sim_instance==nullptr){
        sim_instance = new Simulator();
    }
    return sim_instance;
}

void Simulator::AddSegment( int id, 
                            float x, 
                            float y, 
                            bool has_end_light, 
                            bool has_start_light,
                            bool is_station)
{
    LOG << "Testing AddSegment Logs " << 123;
    LOG_E << "Testing AddSegment Error Logs " << 456;
}