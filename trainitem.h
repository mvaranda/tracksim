// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef ROUTEITEM_H
#define ROUTEITEM_H

#include <QGraphicsPixmapItem>
#include <QList>
#include "simitemid.h"
#include "segment.h"
#include "common.h"

QT_BEGIN_NAMESPACE
class QPixmap;
class QGraphicsSceneContextMenuEvent;
class QMenu;
class QPolygonF;
QT_END_NAMESPACE


//class Segment;

//! [0]
class TrainItem : public QObject, public QGraphicsPixmapItem, public SimItemID
{
    Q_OBJECT

public:
    enum { Type = TRAIN_ITEM_TYPE };
    enum TrainType { Step, Conditional, StartEnd, Io, TrackPoint };

    TrainItem(  QMenu *contextMenu,
                int train_number = 0,
                int sim_id = 0,
                int reverse = 0,
                bool enabled = true,
                QGraphicsItem *parent = nullptr);

    // void removeSegment(Segment *segment);
    void removeSegments();
    // TrainType diagramType() const { return TrainItemType; }
    // QPolygonF polygon() const { return m_Polygon; }
    void addSegment(Segment *segment);
    // QPixmap image() const;
    int type() const override { return Type; }
    void setColor(const QColor &color) { m_Color = color; }
    void getColor(int *r, int * g, int * b) { 
        m_Color.getRgb(r, g, b);
     }
    void setEditing(bool ed);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

    TrainType TrainItemType;
    bool editing;
    QList<Segment *> segments;
    bool enabled;
    bool reverse;
    int train_number;

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
    QPolygonF m_Polygon;
    QMenu *m_ContextMenu;
    QColor m_Color;
    QString text;
    QGraphicsTextItem * txtItem;
    static int train_cnt;

signals:
    void routingStarted(QGraphicsItem *item);
    void routingEnded(QGraphicsItem *item);

};
//! [0]

#endif // ROUTEITEM_H
