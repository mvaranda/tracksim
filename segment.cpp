// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause


#include "segment.h"
#include "diagramitem.h"
#include "common.h"
#include "python_int.h"

#include <QPainter>
#include <QPen>
#include <QtMath>

// Colors demo: https://doc.qt.io/qt-6/qml-color.html
#define SELECTED_COLOR Qt::cyan
//#define ROUTE_COLOR Qt::green //QColorConstants::Svg::blueviolet //Qt::blue
#define ROUTE_COLOR QColorConstants::Svg::blueviolet

#define HAS_TRAIN_COLOR Qt::blue

Segment::Segment(DiagramItem *startItem, DiagramItem *endItem, int sim_id, QGraphicsItem *parent)
    : QGraphicsLineItem(parent), 
        SimItemID(sim_id),
        m_StartItem(startItem), 
        m_EndItem(endItem), 
        trafficLightEnd(TrafficLight::noLight), // GreenLight),
        trafficLightStart(TrafficLight::noLight), //GreenLight),
        showTrain(false),
        hasTrain(false)
{
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setPen(QPen(m_Color, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
}

QRectF Segment::boundingRect() const
{
    qreal extra = (pen().width() + 20) / 2.0;

    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                      line().p2().y() - line().p1().y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

QPainterPath Segment::shape() const
{
    QPainterPath path = QGraphicsLineItem::shape();
    path.addPolygon(segmentHead);
    return path;
}

void Segment::updatePosition()
{
    QLineF line(mapFromItem(m_StartItem, 0, 0), mapFromItem(m_EndItem, 0, 0));
    setLine(line);
}

void Segment::paint(QPainter *painter, const QStyleOptionGraphicsItem * _style, QWidget * _widget)
{
    if (m_StartItem->collidesWithItem(m_EndItem))
        return;

    QPen m_Pen = pen();
    m_Pen.setColor(m_Color);
    qreal segmentSize = 20;
    painter->setPen(m_Pen);
    painter->setBrush(m_Color);


    QLineF centerLine(m_StartItem->pos(), m_EndItem->pos());
    QPolygonF endPolygon = m_EndItem->polygon();
    QPointF p1 = endPolygon.first() + m_EndItem->pos();
    QPointF intersectPoint;
    for (int i = 1; i < endPolygon.count(); ++i) {
        QPointF p2 = endPolygon.at(i) + m_EndItem->pos();
        QLineF polyLine = QLineF(p1, p2);
        QLineF::IntersectionType intersectionType =
            polyLine.intersects(centerLine, &intersectPoint);
        if (intersectionType == QLineF::BoundedIntersection)
            break;
        p1 = p2;
    }

    setLine(QLineF(intersectPoint, m_StartItem->pos()));
    painter->drawLine(line());

    //-------- lights --------
    if (trafficLightEnd == TrafficLight::RedLight) {
        painter->setPen(Qt::red);
        painter->setBrush(Qt::red);
    }
    else {
        painter->setPen(Qt::green);
        painter->setBrush(Qt::green);
    }

    if (trafficLightEnd != TrafficLight::noLight) {
        segmentHead.clear();
        segmentHead << line().p1();
        int i;
        float x,y,a;
        for (i=0; i<360; i += 5) {
            a = (i * 3.1415926) / 180;
            x = segmentSize/2 * sin(a);
            y = segmentSize/2 * cos(a);
            segmentHead << line().p1() + QPointF(x,y);
        }
        painter->drawPolygon(segmentHead);
    }

    if (isSelected()) {
        painter->setPen(QPen(SELECTED_COLOR, 1, Qt::DashLine));
        QLineF m_Line = line();
        m_Line.translate(0, 4.0);
        painter->drawLine(m_Line);
        m_Line.translate(0,-8.0);
        painter->drawLine(m_Line);
        setPen(QPen(Qt::red, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter->drawEllipse(0, 0, 100, 100); // drawEllipse(int x, int y, int width, int height)
    }

    // draw reverse track
    paint_reverse(painter, _style, _widget);

    if (showTrain || hasTrain) {
        painter->setPen(QPen(hasTrain ? HAS_TRAIN_COLOR : ROUTE_COLOR , 4, Qt::SolidLine));
        QLineF m_Line = line();
        m_Line.translate(0, 3.0);
        painter->drawLine(m_Line);
        m_Line.translate(0,-6.0);
        painter->drawLine(m_Line);
        return;        
    }               
}

void Segment::paint_reverse(QPainter *painter, const QStyleOptionGraphicsItem *,
                  QWidget *)
{
    if (m_StartItem->collidesWithItem(m_EndItem))
        return;

    QPen m_Pen = pen();
    m_Pen.setColor(m_Color);
    qreal segmentSize = 20;
    painter->setPen(m_Pen);
    painter->setBrush(m_Color);


    QLineF centerLine(m_EndItem->pos(), m_StartItem->pos());
    QPolygonF endPolygon = m_StartItem->polygon();
    QPointF p1 = endPolygon.first() + m_StartItem->pos();
    QPointF intersectPoint;
    for (int i = 1; i < endPolygon.count(); ++i) {
        QPointF p2 = endPolygon.at(i) + m_StartItem->pos();
        QLineF polyLine = QLineF(p1, p2);
        QLineF::IntersectionType intersectionType =
            polyLine.intersects(centerLine, &intersectPoint);
        if (intersectionType == QLineF::BoundedIntersection)
            break;
        p1 = p2;
    }

    setLine(QLineF(intersectPoint, m_EndItem->pos()));
        painter->drawLine(line());

    if (trafficLightStart == TrafficLight::RedLight) {
        painter->setPen(Qt::red);
        painter->setBrush(Qt::red);
    }
    else {
        painter->setPen(Qt::green);
        painter->setBrush(Qt::green);
    }

    if (trafficLightStart != TrafficLight::noLight) {
        segmentHead.clear();
        segmentHead << line().p1();
        int i;
        float x,y,a;
        for (i=0; i<360; i += 5) {
            a = (i * 3.1415926) / 180;
            x = segmentSize/2 * sin(a);
            y = segmentSize/2 * cos(a);
            segmentHead << line().p1() + QPointF(x,y);
        }
        painter->drawPolygon(segmentHead);
    }

    if (isSelected()) {
        painter->setPen(QPen(SELECTED_COLOR, 1, Qt::DashLine));
        QLineF m_Line = line();
        m_Line.translate(0, 4.0);
        painter->drawLine(m_Line);
        m_Line.translate(0,-8.0);
        painter->drawLine(m_Line);
        setPen(QPen(Qt::red, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter->drawEllipse(0, 0, 100, 100); // drawEllipse(int x, int y, int width, int height)
    }
}

QPointF Segment::getStartPos() {
    return m_StartItem->pos();
}
QPointF Segment::getEndPos() {
    return m_EndItem->pos();
}

void Segment::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "Segment mousePressEvent";
    if (gMode == EditingTrain) {
        showTrain = ! showTrain;
    }

    if (gMode == Simulating) {
        simInt_SendSegmentClick(GetSimItemID());
    }
}
