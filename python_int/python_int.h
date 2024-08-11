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

#define SIMULATOR_TIME_TICK  10  // 10 ticks per second
#define TRAIN_DEFAULT_SPEED  5   // ticks per segment
#define TRAIN_DEFAULT_START_TIME  5 // ticks

#define MAX_NUM_SEGS_PER_TRACKPOINT 4
#define NUM_MAX_SEGMENTS_PER_ROUTE  64

typedef struct item_st {
  char      name[32];
  char      type[16];
  int       sim_id;
  int       pos_x; 
  int       pos_y;
  int       color_r;
  int       color_g;
  int       color_b;
  int       segment_id[MAX_NUM_SEGS_PER_TRACKPOINT]; // dic in py side gets key names: segment_id_0 to segment_id_3

} item_t;

typedef struct segment_st {
  char      name[32];
  char      type[16];
  int       sim_id;
  int       pos_x;
  int       pos_y;
  int       color_r;
  int       color_g;
  int       color_b;
  int       startTrackPoint_id;
  int       endTrackPoint_id;
  int       startLightState;
  int       endLightState;
} segment_t;

typedef struct text_st {
  char      text[64];
  char      font_name[64];
  int       size;
  int       pos_x; 
  int       pos_y;
  int       color_r;
  int       color_g;
  int       color_b;
} text_t;

typedef struct train_st {
  int       train_number;
  int       sim_id;
  int       pos_x;
  int       pos_y;
  int       speed;      // ticks/second
  int       enabled;
  int       reverse;
  int       start_time;  // ticks
  int       route_seg_ids[NUM_MAX_SEGMENTS_PER_ROUTE];
} train_t;

#ifdef __cplusplus
  extern "C" {
#endif

bool simInt_init(const char * _program);
void simInt_clearAll();
bool simInt_addItem(item_t * item);
bool simInt_addText(text_t * txt);
bool simInt_addSegment(segment_t * seg);
bool simInt_addTrain(train_t * train);

void simInt_add(int x, int y);

void simInt_get_val(const char * var_name);

void simInt_destroy();

bool simInt_save(const char * file);
bool simInt_load(const char * file);
bool simInt_start();
bool simInt_timer_tick();

#ifdef __cplusplus
  }
#endif