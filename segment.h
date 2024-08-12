// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef SEGMENT_H
#define SEGMENT_H

#include <QGraphicsLineItem>
#include "simitemid.h"
#include "common.h"

class DiagramItem;

//! [0]
class Segment : public QObject, public QGraphicsLineItem, public SimItemID
{
public:
    enum { Type = ARROW_ITEM_TYPE };
    enum TrafficLight { noLight, GreenLight, RedLight};

    Segment(DiagramItem *startItem, DiagramItem *endItem,  int sim_id = 0,
          QGraphicsItem *parent = nullptr);

    int type() const override { return Type; }
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void setColor(const QColor &color) { m_Color = color; }
    void getColor(int *r, int * g, int * b) { 
        m_Color.getRgb(r, g, b);
     }
    DiagramItem *startItem() const { return m_StartItem; }
    DiagramItem *endItem() const { return m_EndItem; }

    void updatePosition();

    TrafficLight trafficLightStart;
    TrafficLight trafficLightEnd;

    QPointF getStartPos();
    QPointF getEndPos();

    bool showTrain;
    bool hasTrain;
    bool showRed;

    void mousePressEvent(QGraphicsSceneMouseEvent *event);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = nullptr) override;

private:
    DiagramItem *m_StartItem;
    DiagramItem *m_EndItem;
    QPolygonF segmentHead;
    QColor m_Color = Qt::black;
    void paint_reverse(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
};
//! [0]

#endif // SEGMENT_H
