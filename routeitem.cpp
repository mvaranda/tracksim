// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "routeitem.h"
//#include "arrow.h"

#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>
#include <QDebug>

#define TRACK_POINT_CIRCLE_SIZE 20.0

int RouteItem::route_cnt = 0;

//! [0]
RouteItem::RouteItem(QMenu *contextMenu, int sim_id,
                         QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent), 
    SimItemID(sim_id),
    m_ContextMenu(contextMenu)
{
    router_number = ++route_cnt;
    setEditing(true);
    //QPixmap img(":/images/route.png");
    // QPixmap img(":/images/route_editing.png");
    // QPixmap _img = img.scaled(QSize(60,60));
    // setPixmap(_img);

    QString t = "Route ";
    t += QString::number(router_number);
    txtItem = new QGraphicsTextItem(t, this);
    txtItem->setPos(0,56);
    QFont f = txtItem->font();
    f.setPointSize(10);
    txtItem->setFont(f);
    
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

void RouteItem::setEditing(bool ed)
{
    QPixmap img;
    if (ed) {
        img = QPixmap(":/images/route_editing.png");
    }
    else {
        img = QPixmap(":/images/route.png");
    }

    QPixmap _img = img.scaled(QSize(60,60));
    setPixmap(_img);
    editing = ed;
}


// void RouteItem::removeArrow(Arrow *arrow)
// {
//     arrows.removeAll(arrow);
// }


// void RouteItem::removeArrows()
// {
//     // need a copy here since removeArrow() will
//     // modify the arrows container
//     const auto arrowsCopy = arrows;
//     for (Arrow *arrow : arrowsCopy) {
//         arrow->startItem()->removeArrow(arrow);
//         arrow->endItem()->removeArrow(arrow);
//         scene()->removeItem(arrow);
//         delete arrow;
//     }
// }


// void RouteItem::addArrow(Arrow *arrow)
// {
//     arrows.append(arrow);
// }


void RouteItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);
    m_ContextMenu->popup(event->screenPos());
}

QVariant RouteItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionChange) {
        // for (Arrow *arrow : std::as_const(arrows))
        //     arrow->updatePosition();
    }

    return value;
}

void RouteItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (editing) {
        setEditing(false);
        emit routingEnded(this);
    }
}

void RouteItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{

}

