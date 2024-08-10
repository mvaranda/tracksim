// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef DIAGRAMITEM_H
#define DIAGRAMITEM_H

#include <QGraphicsPixmapItem>
#include <QList>
#include "simitemid.h"
#include "common.h"

QT_BEGIN_NAMESPACE
class QPixmap;
class QGraphicsSceneContextMenuEvent;
class QMenu;
class QPolygonF;
QT_END_NAMESPACE

class Segment;

//! [0]
class DiagramItem : public QGraphicsPolygonItem, public SimItemID
{
public:
    enum { Type = DIAG_ITEM_TYPE };
    enum DiagramType { Step, Conditional, StartEnd, Io, TrackPoint };

    DiagramItem(DiagramType diagramType, QMenu *contextMenu, int sim_id = 0, QGraphicsItem *parent = nullptr);

    void removeSegment(Segment *segment);
    void removeSegments();
    DiagramType diagramType() const { return diagramItemType; }
    QPolygonF polygon() const { return m_Polygon; }
    void addSegment(Segment *segment);
    QPixmap image() const;
    int type() const override { return Type; }
    void setColor(const QColor &color) { m_Color = color; }
    void getColor(int *r, int * g, int * b) { 
        m_Color.getRgb(r, g, b);
     }


    DiagramType diagramItemType;
    QList<Segment *> segments;

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
    QPolygonF m_Polygon;
    QMenu *m_ContextMenu;
    QColor m_Color;

};
//! [0]

#endif // DIAGRAMITEM_H
