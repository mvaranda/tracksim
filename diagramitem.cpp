// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "diagramitem.h"
#include "segment.h"

#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>
#include <QDebug>

#define TRACK_POINT_CIRCLE_SIZE 12.0

//! [0]
DiagramItem::DiagramItem(DiagramType diagramType, QMenu *contextMenu, int sim_id,
                         QGraphicsItem *parent)
    : QGraphicsPolygonItem(parent), 
    SimItemID(sim_id),
    diagramItemType(diagramType)
    , m_ContextMenu(contextMenu)
{
    QPainterPath path;
    switch (diagramItemType) {
        case StartEnd:
            path.moveTo(200, 50);
            path.arcTo(150, 0, 50, 50, 0, 90);
            path.arcTo(50, 0, 50, 50, 90, 90);
            path.arcTo(50, 50, 50, 50, 180, 90);
            path.arcTo(150, 50, 50, 50, 270, 90);
            path.lineTo(200, 25);
            m_Polygon = path.toFillPolygon();
            break;
        case Conditional:
            m_Polygon << QPointF(-100, 0) << QPointF(0, 100)
                      << QPointF(100, 0) << QPointF(0, -100)
                      << QPointF(-100, 0);
            break;
        case Step:
            m_Polygon << QPointF(-100, -100) << QPointF(100, -100)
                      << QPointF(100, 100) << QPointF(-100, 100)
                      << QPointF(-100, -100);
            break;
        case TrackPoint:
            int i;
            float x,y,a;
            for (i=0; i<360; i += 5) {
                a = (i * 3.1415926) / 180;
                x = TRACK_POINT_CIRCLE_SIZE * sin(a);
                y = TRACK_POINT_CIRCLE_SIZE * cos(a);
                m_Polygon << QPointF(x,y);
            }
            break;

        default:
            m_Polygon << QPointF(-120, -80) << QPointF(-70, 80)
                      << QPointF(120, 80) << QPointF(70, -80)
                      << QPointF(-120, -80);
            break;
    }
    setPolygon(m_Polygon);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}
//! [0]

//! [1]
void DiagramItem::removeSegment(Segment *segment)
{
    segments.removeAll(segment);
}
//! [1]

//! [2]
void DiagramItem::removeSegments()
{
    // need a copy here since removeSegment() will
    // modify the segments container
    const auto segmentsCopy = segments;
    for (Segment *segment : segmentsCopy) {
        segment->startItem()->removeSegment(segment);
        segment->endItem()->removeSegment(segment);
        scene()->removeItem(segment);
        delete segment;
    }
}
//! [2]

//! [3]
void DiagramItem::addSegment(Segment *segment)
{
    segments.append(segment);
}
//! [3]

//! [4]
QPixmap DiagramItem::image() const
{
    QPixmap pixmap(250, 250);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setPen(QPen(Qt::black, 8));
    painter.translate(125, 125);
    if (diagramItemType == DiagramType::TrackPoint)
        painter.scale(3.0, 3.0);
    painter.drawPolyline(m_Polygon);

    return pixmap;
}
//! [4]

//! [5]
void DiagramItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);
    m_ContextMenu->popup(event->screenPos());
}
//! [5]

//! [6]
QVariant DiagramItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionChange) {
        for (Segment *segment : std::as_const(segments))
            segment->updatePosition();
    }

    return value;
}
//! [6]
