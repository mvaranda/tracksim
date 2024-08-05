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

#define MAX_NUM_SEGS_PER_TRACKPOINT 4

typedef struct item_st {
  char      name[32];
  char      type[16];
  int       sim_id;
  float     pos_x; 
  float     pos_y;
  int       segment_id[MAX_NUM_SEGS_PER_TRACKPOINT]; // dic in py side gets key names: segment_id_0 to segment_id_3

} item_t;

typedef struct segment_st {
  char      name[32];
  char      type[16];
  int       sim_id;
  float     pos_x;
  float     pos_y;
  int       startTrackPoint_id;
  int       endTrackPoint_id;
  int       startLightState;
  int       endLightState;
} segment_t;

#ifdef __cplusplus
  extern "C" {
#endif

void simInt_init(const char * _program);
void simInt_clearAll();
bool simInt_addItem(item_t * item);
bool simInt_addSegment(segment_t * seg);

void simInt_add(int x, int y);

void simInt_get_val(const char * var_name);

void simInt_destroy();

#ifdef __cplusplus
  }
#endif