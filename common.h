
#pragma once 

typedef enum {  InsertItem, 
                InsertLine, 
                InsertText, 
                MoveItem, 
                InsertRoute,
                EditingRoute,
                Simulating } gMode_t;

extern gMode_t gMode;

void setGMode(int m);