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
        m_StartItem(startItem), 
        m_EndItem(endItem), 
        trafficLightEnd(TrafficLight::GreenLight),
        trafficLightStart(TrafficLight::GreenLight)
{
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setPen(QPen(m_Color, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
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
    QLineF line(mapFromItem(m_StartItem, 0, 0), mapFromItem(m_EndItem, 0, 0));
    setLine(line);
}
//! [3]

//! [4]
void Arrow::paint(QPainter *painter, const QStyleOptionGraphicsItem * _style, QWidget * _widget)
{
    if (m_StartItem->collidesWithItem(m_EndItem))
        return;

    QPen m_Pen = pen();
    m_Pen.setColor(m_Color);
    qreal arrowSize = 20;
    painter->setPen(m_Pen);
    painter->setBrush(m_Color);
//! [4] //! [5]


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
                  
}
//! [7]

void Arrow::paint_reverse(QPainter *painter, const QStyleOptionGraphicsItem *,
                  QWidget *)
{
    if (m_StartItem->collidesWithItem(m_EndItem))
        return;

    QPen m_Pen = pen();
    m_Pen.setColor(m_Color);
    qreal arrowSize = 20;
    painter->setPen(m_Pen);
    painter->setBrush(m_Color);
//! [4] //! [5]


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
        QLineF m_Line = line();
        m_Line.translate(0, 4.0);
        painter->drawLine(m_Line);
        m_Line.translate(0,-8.0);
        painter->drawLine(m_Line);
        setPen(QPen(Qt::red, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter->drawEllipse(0, 0, 100, 100); // drawEllipse(int x, int y, int width, int height)
    }
}

    QPointF Arrow::getStartPos() {
        return m_StartItem->pos();
    }
    QPointF Arrow::getEndPos() {
        return m_EndItem->pos();
    }
