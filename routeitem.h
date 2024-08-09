// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef ROUTEITEM_H
#define ROUTEITEM_H

#include <QGraphicsPixmapItem>
#include <QList>
#include "simitemid.h"

QT_BEGIN_NAMESPACE
class QPixmap;
class QGraphicsSceneContextMenuEvent;
class QMenu;
class QPolygonF;
QT_END_NAMESPACE

//class Arrow;

//! [0]
class RouteItem : public QGraphicsPixmapItem, public SimItemID
{
//    Q_OBJECT

public:
    enum { Type = UserType + 16 };
    enum RouteType { Step, Conditional, StartEnd, Io, TrackPoint };

    RouteItem(QMenu *contextMenu, int sim_id = 0, QGraphicsItem *parent = nullptr);

    // void removeArrow(Arrow *arrow);
    // void removeArrows();
    // RouteType diagramType() const { return RouteItemType; }
    // QPolygonF polygon() const { return m_Polygon; }
    // void addArrow(Arrow *arrow);
    // QPixmap image() const;
    int type() const override { return Type; }
    void setColor(const QColor &color) { m_Color = color; }
    void getColor(int *r, int * g, int * b) { 
        m_Color.getRgb(r, g, b);
     }
    void setEditing(bool ed);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

    RouteType RouteItemType;
    bool editing;
//    QList<Arrow *> arrows;

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
    QPolygonF m_Polygon;
    QMenu *m_ContextMenu;
    QColor m_Color;
    QString text;
    QGraphicsTextItem * txtItem;
    int router_number;
    static int route_cnt;

// signals:
//     void routingEnded(QGraphicsItem *item);

};
//! [0]

#endif // ROUTEITEM_H
