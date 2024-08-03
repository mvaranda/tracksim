// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause


#include "arrow.h"
#include "diagramitem.h"

#include <QPainter>
#include <QPen>
#include <QtMath>

#define SELECTED_COLOR Qt::cyan

//! [0]
Arrow::Arrow(DiagramItem *startItem, DiagramItem *endItem, QGraphicsItem *parent)
    : QGraphicsLineItem(parent), 
        myStartItem(startItem), 
        myEndItem(endItem), 
        trafficLightEnd(TrafficLight::GreenLight),
        trafficLightStart(TrafficLight::GreenLight)
{
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setPen(QPen(myColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
}
//! [0]

//! [1]
QRectF Arrow::boundingRect() const
{
    qreal extra = (pen().width() + 20) / 2.0;

    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                      line().p2().y() - line().p1().y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}
//! [1]

//! [2]
QPainterPath Arrow::shape() const
{
    QPainterPath path = QGraphicsLineItem::shape();
    path.addPolygon(arrowHead);
    return path;
}
//! [2]

//! [3]
void Arrow::updatePosition()
{
    QLineF line(mapFromItem(myStartItem, 0, 0), mapFromItem(myEndItem, 0, 0));
    setLine(line);
}
//! [3]

//! [4]
void Arrow::paint(QPainter *painter, const QStyleOptionGraphicsItem * _style, QWidget * _widget)
{
    if (myStartItem->collidesWithItem(myEndItem))
        return;

    QPen myPen = pen();
    myPen.setColor(myColor);
    qreal arrowSize = 20;
    painter->setPen(myPen);
    painter->setBrush(myColor);
//! [4] //! [5]


    QLineF centerLine(myStartItem->pos(), myEndItem->pos());
    QPolygonF endPolygon = myEndItem->polygon();
    QPointF p1 = endPolygon.first() + myEndItem->pos();
    QPointF intersectPoint;
    for (int i = 1; i < endPolygon.count(); ++i) {
        QPointF p2 = endPolygon.at(i) + myEndItem->pos();
        QLineF polyLine = QLineF(p1, p2);
        QLineF::IntersectionType intersectionType =
            polyLine.intersects(centerLine, &intersectPoint);
        if (intersectionType == QLineF::BoundedIntersection)
            break;
        p1 = p2;
    }

    setLine(QLineF(intersectPoint, myStartItem->pos()));
        painter->drawLine(line());

    if (trafficLightEnd == TrafficLight::RedLight) {
        painter->setPen(Qt::red);
        painter->setBrush(Qt::red);
    }
    else {
        painter->setPen(Qt::green);
        painter->setBrush(Qt::green);
    }
//! [5] //! [6]
    if (trafficLightEnd != TrafficLight::noLight) {
        arrowHead.clear();
        arrowHead << line().p1();
        int i;
        float x,y,a;
        for (i=0; i<360; i += 5) {
            a = (i * 3.1415926) / 180;
            x = arrowSize/2 * sin(a);
            y = arrowSize/2 * cos(a);
            arrowHead << line().p1() + QPointF(x,y);
        }
        painter->drawPolygon(arrowHead);
    }

    if (isSelected()) {
        painter->setPen(QPen(SELECTED_COLOR, 1, Qt::DashLine));
        QLineF myLine = line();
        myLine.translate(0, 4.0);
        painter->drawLine(myLine);
        myLine.translate(0,-8.0);
        painter->drawLine(myLine);
        setPen(QPen(Qt::red, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter->drawEllipse(0, 0, 100, 100); // drawEllipse(int x, int y, int width, int height)
    }

    // draw reverse track
    paint_reverse(painter, _style, _widget);
                  
}
//! [7]

void Arrow::paint_reverse(QPainter *painter, const QStyleOptionGraphicsItem *,
                  QWidget *)
{
    if (myStartItem->collidesWithItem(myEndItem))
        return;

    QPen myPen = pen();
    myPen.setColor(myColor);
    qreal arrowSize = 20;
    painter->setPen(myPen);
    painter->setBrush(myColor);
//! [4] //! [5]


    QLineF centerLine(myEndItem->pos(), myStartItem->pos());
    QPolygonF endPolygon = myStartItem->polygon();
    QPointF p1 = endPolygon.first() + myStartItem->pos();
    QPointF intersectPoint;
    for (int i = 1; i < endPolygon.count(); ++i) {
        QPointF p2 = endPolygon.at(i) + myStartItem->pos();
        QLineF polyLine = QLineF(p1, p2);
        QLineF::IntersectionType intersectionType =
            polyLine.intersects(centerLine, &intersectPoint);
        if (intersectionType == QLineF::BoundedIntersection)
            break;
        p1 = p2;
    }

    setLine(QLineF(intersectPoint, myEndItem->pos()));
        painter->drawLine(line());

    if (trafficLightEnd == TrafficLight::RedLight) {
        painter->setPen(Qt::red);
        painter->setBrush(Qt::red);
    }
    else {
        painter->setPen(Qt::green);
        painter->setBrush(Qt::green);
    }
//! [5] //! [6]
    if (trafficLightEnd != TrafficLight::noLight) {
        arrowHead.clear();
        arrowHead << line().p1();
        int i;
        float x,y,a;
        for (i=0; i<360; i += 5) {
            a = (i * 3.1415926) / 180;
            x = arrowSize/2 * sin(a);
            y = arrowSize/2 * cos(a);
            arrowHead << line().p1() + QPointF(x,y);
        }
        painter->drawPolygon(arrowHead);
    }

    if (isSelected()) {
        painter->setPen(QPen(SELECTED_COLOR, 1, Qt::DashLine));
        QLineF myLine = line();
        myLine.translate(0, 4.0);
        painter->drawLine(myLine);
        myLine.translate(0,-8.0);
        painter->drawLine(myLine);
        setPen(QPen(Qt::red, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter->drawEllipse(0, 0, 100, 100); // drawEllipse(int x, int y, int width, int height)
    }
}

    QPointF Arrow::getStartPos() {
        return myStartItem->pos();
    }
    QPointF Arrow::getEndPos() {
        return myEndItem->pos();
    }
