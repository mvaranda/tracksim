// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "trainitem.h"
//#include "segment.h"

#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>
#include <QDebug>

#define TRACK_POINT_CIRCLE_SIZE 20.0

int TrainItem::train_cnt = 0;

TrainItem::TrainItem(   QMenu *contextMenu,
                        int sim_id,
                        int _reverse,
                        bool _enabled,
                        QGraphicsItem *parent)

    : QGraphicsPixmapItem(parent), 
    SimItemID(sim_id),
    m_ContextMenu(contextMenu),
    reverse(_reverse),
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

void TrainItem::removeSegments()
{
    segments.clear();
}

void TrainItem::addSegment(Segment *segment)
{
    segments.append(segment);
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
        // for (Segment *segment : std::as_const(segments))
        //     segment->updatePosition();
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
    foreach( Segment *segment, segments ) {
        segment->showTrain = true;
    }
    segments.clear();
    emit routingStarted(this);
}

