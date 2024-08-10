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

#include "store.h"
#include <QDebug>

// #include "rapidjson/document.h" 
// #include "rapidjson/filewritestream.h" 
// #include "rapidjson/writer.h"
// #include "rapidjson/prettywriter.h"
// #include <fstream> 


/**********************  Example ********************

{
  "items": [
    {
      "sim_id:": 10,
      "type": "trackpoint",
      "pos_x": 1234.5,
      "pos_y": 3456.8,
      "segments": [
        {
          "sim_id:": 12
        },
        {
          "sim_id:": 13
        }
      ]
    },
    {
      "sim_id:": 11,
      "type": "trackpoint",
      "pos_x": 1234.5,
      "pos_y": 3456.8
    }
  ],
  "segments": [
    {
      "sim_id:": 12,
      "type": "segment",
      "pos_x": 1234.5,
      "pos_y": 3456.8,
      "startTrackPoint_id": 1,
      "endTrackPoint_id": 2,
      "startLightState": 0,
      "endLightState": 0
    },
    {
      "sim_id:": 13,
      "type": "segment",
      "pos_x": 1234.5,
      "pos_y": 3456.8,
      "startTrackPoint_id": 1,
      "endTrackPoint_id": 2,
      "startLightState": 0,
      "endLightState": 0
    }
  ]
}
*/

bool Store::SaveStart(const std::string name)
{
  qDebug() << "SaveStart called, filename: " << name;

  return true;
}

bool Store::SaveAddItem(  std::string name,
                          std::string type,
                          int sim_id, 
                          float pos_x, 
                          float pos_y,
                          const std::vector<int> & segment_ids)
{
  qDebug() << "SaveAddItem called";

  return true;
}

bool Store::SaveAddSegment( std::string name,
                            std::string type,
                            int sim_id, 
                            float pos_x, 
                            float pos_y,
                            int startTrackPoint_id,
                            int endTrackPoint_id,
                            int startLightState,
                            int endLightState)
{
  qDebug() << "SaveAddSegment called";

  return true;
}

bool Store::SaveFinish()
{
  qDebug() << "SaveFinish called";

  return true;
}






