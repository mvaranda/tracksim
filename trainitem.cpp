// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "trainitem.h"
//#include "arrow.h"

#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>
#include <QDebug>

#define TRACK_POINT_CIRCLE_SIZE 20.0

int TrainItem::train_cnt = 0;

TrainItem::TrainItem(   QMenu *contextMenu,
                        int sim_id,
                        int _firstArrow,
                        bool _enabled,
                        QGraphicsItem *parent)

    : QGraphicsPixmapItem(parent), 
    SimItemID(sim_id),
    m_ContextMenu(contextMenu),
    firstArrow(_firstArrow),
    enabled(_enabled)
{
    trainr_number = ++train_cnt;
    setEditing(true);
    //QPixmap img(":/images/train.png");
    // QPixmap img(":/images/train_editing.png");
    // QPixmap _img = img.scaled(QSize(60,60));
    // setPixmap(_img);

    QString t = "Train ";
    t += QString::number(trainr_number);
    txtItem = new QGraphicsTextItem(t, this);
    txtItem->setPos(0,56);
    QFont f = txtItem->font();
    f.setPointSize(10);
    txtItem->setFont(f);
    
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

void TrainItem::setEditing(bool ed)
{
    QPixmap img;
    if (ed) {
        img = QPixmap(":/images/train_editing.png");
    }
    else {
        img = QPixmap(":/images/train.png");
    }

    QPixmap _img = img.scaled(QSize(60,60));
    setPixmap(_img);
    editing = ed;
}

void TrainItem::removeArrows()
{
    arrows.clear();
}

void TrainItem::addArrow(Arrow *arrow)
{
    arrows.append(arrow);
}

void TrainItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);
    m_ContextMenu->popup(event->screenPos());
}

QVariant TrainItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionChange) {
        // for (Arrow *arrow : std::as_const(arrows))
        //     arrow->updatePosition();
    }

    return value;
}

void TrainItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (editing) {
        setEditing(false);
        emit routingEnded(this);
    }
}

void TrainItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "Double click";
    if (editing || gMode == ::EditingTrain) {
        // already editing
        qDebug("Already editing");
        return;
    }
    
    setEditing(true);
    foreach( Arrow *arrow, arrows ) {
        arrow->showTrain = true;
    }
    arrows.clear();
    emit routingStarted(this);
}

