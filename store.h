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



#include <vector>
#include <iostream>

class Store {
public:
  Store(): fh(nullptr){}

  bool SaveStart(const std::string name);
  bool SaveAddItem( std::string name,
                    std::string type,
                    int sim_id, 
                    float pos_x, 
                    float pos_y,
                    const std::vector<int> & segment_ids);

  bool SaveAddSegment(  std::string name,
                        std::string type,
                        int sim_id, 
                        float pos_x, 
                        float pos_y,
                        int startTrackPoint_id,
                        int endTrackPoint_id,
                        int startLightState,
                        int endLightState);
  bool SaveFinish();

private:
  FILE * fh;
};