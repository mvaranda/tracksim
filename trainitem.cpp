// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "trainitem.h"
#include "python_int.h"

#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>
#include <QDebug>

#define TRACK_POINT_CIRCLE_SIZE 20.0

int TrainItem::train_cnt = 0;
TrainItem * TrainItem::trainEditing = NULL;

TrainItem::TrainItem(   QMenu *contextMenu,
                        int _train_number,
                        int sim_id,
                        int _reverse,
                        bool _enabled,
                        QGraphicsItem *parent)

    : QGraphicsPixmapItem(parent), 
    SimItemID(sim_id),
    m_ContextMenu(contextMenu), 
    enabled(_enabled),
    reverse(_reverse),
    train_number(_train_number),
    speed(TRAIN_DEFAULT_SPEED)
{

    if (! train_number) {
        // User is adding a train
        train_number = ++train_cnt;
        setEditing(true);
    }
    else {
        // loader is adding, so no in edit mode
        setEditing(false);
    }


    QString t = "Train ";
    t += QString::number(train_number);
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
        trainEditing = this;
    }
    else {
        img = QPixmap(":/images/train.png");
        trainEditing = NULL;
    }

    QPixmap _img = img.scaled(QSize(60,60));
    setPixmap(_img);
    editing = ed;
}

void TrainItem::removeSegments()
{
    qDebug() << "removeSegments to be removed";
    //segments.clear();
}

void TrainItem::addSegment(Segment *segment)
{
    segments.append(segment);
}

// int searchSegment(QList * list, int id)
// {
//     bool found = false;
//     int i = 0;
//     for (auto s in list) {
//         if (s == id) {
//             return i;
//         }
//         i++;
//     }
//     return -1;
// }

void TrainItem::updateSegment(Segment * seg, bool add)
{
    int seg_sim_id = seg->GetSimItemID();
    int i = 0;
    bool found = false;

    if (add) {
        foreach (auto s, segments) {
            if (s == seg) {
                found = true;
                break;
            }
        }
        if (!found) {
            qDebug( ) << "segments size before adding" << segments.size();
            segments.append(seg);
            qDebug( ) << "adding segment id = " << seg_sim_id;
            qDebug( ) << "segments size after adding" << segments.size();
        }
    } else {
        foreach (auto s, segments) {
            if (s == seg) {
                found = true;
                break;
            }
            i++;
        }
        if (found) {
            qDebug( ) << "segments size before remove" << segments.size();
            segments.remove(i);
            qDebug( ) << "removing segment id = " << seg_sim_id;
            qDebug( ) << "segments size after remove" << segments.size();
        }
    }
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

    }
    // TODO: remove later
    emit routingEnded(this);
    // foreach( Segment *segment, segments ) {
    //     segment->showTrain = false;
    //}
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
    // TODO: clean up later
    // segments.clear();
    routingStarted(this);
}

