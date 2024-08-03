// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef ARROW_H
#define ARROW_H

#include <QGraphicsLineItem>
#include "simitemid.h"

class DiagramItem;

//! [0]
class Arrow : public QGraphicsLineItem, public SimItemID
{
public:
    enum { Type = UserType + 4 };
    enum TrafficLight { noLight, GreenLight, RedLight};

    Arrow(DiagramItem *startItem, DiagramItem *endItem,
          QGraphicsItem *parent = nullptr);

    int type() const override { return Type; }
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void setColor(const QColor &color) { __Color = color; }
    DiagramItem *startItem() const { return __StartItem; }
    DiagramItem *endItem() const { return __EndItem; }

    void updatePosition();

    TrafficLight trafficLightStart;
    TrafficLight trafficLightEnd;

    QPointF getStartPos();
    QPointF getEndPos();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = nullptr) override;

private:
    DiagramItem *__StartItem;
    DiagramItem *__EndItem;
    QPolygonF arrowHead;
    QColor __Color = Qt::black;
    void paint_reverse(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
};
//! [0]

#endif // ARROW_H
