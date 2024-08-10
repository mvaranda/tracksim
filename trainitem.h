// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef ROUTEITEM_H
#define ROUTEITEM_H

#include <QGraphicsPixmapItem>
#include <QList>
#include "simitemid.h"
#include "arrow.h"
#include "common.h"

QT_BEGIN_NAMESPACE
class QPixmap;
class QGraphicsSceneContextMenuEvent;
class QMenu;
class QPolygonF;
QT_END_NAMESPACE


//class Arrow;

//! [0]
class TrainItem : public QObject, public QGraphicsPixmapItem, public SimItemID
{
    Q_OBJECT

public:
    enum { Type = TRAIN_ITEM_TYPE };
    enum TrainType { Step, Conditional, StartEnd, Io, TrackPoint };

    TrainItem(  QMenu *contextMenu, 
                int sim_id = 0,
                int firstArrow = 0,
                bool enabled = true,
                QGraphicsItem *parent = nullptr);

    // void removeArrow(Arrow *arrow);
    void removeArrows();
    // TrainType diagramType() const { return TrainItemType; }
    // QPolygonF polygon() const { return m_Polygon; }
    void addArrow(Arrow *arrow);
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
    QList<Arrow *> arrows;
    bool enabled;
    int firstArrow;

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
    QPolygonF m_Polygon;
    QMenu *m_ContextMenu;
    QColor m_Color;
    QString text;
    QGraphicsTextItem * txtItem;
    int trainr_number;
    static int train_cnt;

signals:
    void routingStarted(QGraphicsItem *item);
    void routingEnded(QGraphicsItem *item);

};
//! [0]

#endif // ROUTEITEM_H
