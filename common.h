
#pragma once 
#include "diagramscene.h"

// mirror of DiagramScene::Mode

typedef enum {  InsertItem, 
                InsertLine, 
                InsertText, 
                MoveItem, 
                InsertRoute,
                EditingRoute,
                Simulating } gMode_t;

extern gMode_t gMode;

void setGMode(int m);