
#pragma once 

#define TEXT_ITEM_TYPE (QGraphicsTextItem::UserType + 3)
#define ARROW_ITEM_TYPE (QGraphicsLineItem::UserType + 4)
#define DIAG_ITEM_TYPE (QGraphicsPolygonItem::UserType + 15)
#define TRAIN_ITEM_TYPE (QGraphicsPixmapItem::UserType + 16)


typedef enum {  InsertItem, 
                InsertLine, 
                InsertText, 
                MoveItem, 
                InsertTrain,
                EditingTrain,
                Simulating } gMode_t;

extern gMode_t gMode;

void setGMode(int m);