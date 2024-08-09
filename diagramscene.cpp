// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "diagramscene.h"
#include "arrow.h"
#include "store.h"

#include <QGraphicsSceneMouseEvent>
#include <QTextCursor>
#include <QFile>
#include <QDebug>
#include "python_int.h"

DiagramScene::DiagramScene(QMenu *itemMenu, QObject *parent)
    : QGraphicsScene(parent)
{
    m_ItemMenu = itemMenu;
    setMode(MoveItem);
    m_ItemType = DiagramItem::Step;
    line = nullptr;
    textItem = nullptr;
    m_ItemColor = DEFAULT_ITEM_COLOR;
    m_TextColor = Qt::black;
    m_LineColor = Qt::black;
}

void DiagramScene::setLineColor(const QColor &color)
{
    m_LineColor = color;
    if (isItemChange(Arrow::Type)) {
        Arrow *item = qgraphicsitem_cast<Arrow *>(selectedItems().first());
        item->setColor(m_LineColor);
        update();
    }
}
//! [1]

//! [2]
void DiagramScene::setTextColor(const QColor &color)
{
    m_TextColor = color;
    if (isItemChange(DiagramTextItem::Type)) {
        DiagramTextItem *item = qgraphicsitem_cast<DiagramTextItem *>(selectedItems().first());
        item->setDefaultTextColor(m_TextColor);
    }
}
//! [2]

//! [3]
void DiagramScene::setItemColor(const QColor &color)
{
    m_ItemColor = color;
    if (isItemChange(DiagramItem::Type)) {
        DiagramItem *item = qgraphicsitem_cast<DiagramItem *>(selectedItems().first());

        item->setBrush(m_ItemColor);
        item->setColor(color);
    }
}
//! [3]

//! [4]
void DiagramScene::setFont(const QFont &font)
{
    m_Font = font;

    if (isItemChange(DiagramTextItem::Type)) {
        QGraphicsTextItem *item = qgraphicsitem_cast<DiagramTextItem *>(selectedItems().first());
        //At this point the selection can change so the first selected item might not be a DiagramTextItem
        if (item)
            item->setFont(m_Font);
    }
}
//! [4]

void DiagramScene::setMode(Mode mode)
{
    m_Mode = mode;
    setGMode(mode);
}

void DiagramScene::setItemType(DiagramItem::DiagramType type)
{
    m_ItemType = type;
}

//! [5]
void DiagramScene::editorLostFocus(DiagramTextItem *item)
{
    QTextCursor cursor = item->textCursor();
    cursor.clearSelection();
    item->setTextCursor(cursor);

    if (item->toPlainText().isEmpty()) {
        removeItem(item);
        item->deleteLater();
    }
}
//! [5]

void DiagramScene::routingHasStarted(QGraphicsItem *_item)
{
    qDebug("got routingHasStarted Signal");
    // TrainItem *trainitem = qgraphicsitem_cast<TrainItem *>(_item);
    // foreach( QGraphicsItem *it, items() ) {
    //     if (it->type() == Arrow::Type) {
    //         Arrow *arrow = qgraphicsitem_cast<Arrow *>(it);
    //         if (arrow->showTrain) {
    //             trainitem->addArrow(arrow);
    //             arrow->showTrain = false;
    //         }
    //     }
    // }
    setMode(EditingTrain);
    invalidate();
}

void DiagramScene::routingHasEnded(QGraphicsItem *_item)
{
    qDebug("got routingHasEnded Signal");
    TrainItem *trainitem = qgraphicsitem_cast<TrainItem *>(_item);
    foreach( QGraphicsItem *it, items() ) {
        if (it->type() == Arrow::Type) {
            Arrow *arrow = qgraphicsitem_cast<Arrow *>(it);
            if (arrow->showTrain) {
                trainitem->addArrow(arrow);
                arrow->showTrain = false;
            }
        }
    }
    setMode(MoveItem);
    invalidate();
}

void DiagramScene::loadScene(QString data)
{
    // 3 TrackPoints:
    QPoint p1(2418 , 2346); 
    QPoint p2(2539 , 2513);
    QPoint p3(2275 , 2507);

    AddItem( DiagramItem::TrackPoint,
                            m_ItemMenu,
                            p1,
                            Qt::blue,
                            5);

    AddItem( DiagramItem::TrackPoint,
                            m_ItemMenu,
                            p2,
                            Qt::green);

    AddItem( DiagramItem::TrackPoint,
                            m_ItemMenu,
                            p3,
                            Qt::red);
}

void DiagramScene::AddItem( DiagramItem::DiagramType itemType,
                            QMenu * itemMenu,
                            QPointF pos,
                            QColor color,
                            int sim_id)
{
    DiagramItem *item;
    item = new DiagramItem(itemType, itemMenu, sim_id);
    item->setBrush(color);
    item->setColor(color);
    addItem(item);
    item->setPos(pos);
    emit itemInserted(item);
}

void DiagramScene::AddSegment(segment_t * seg)
{
    // QPoint p(seg->pos_x , seg->pos_y);
    QColor color;
    color.setRgb(seg->color_r, seg->color_g, seg->color_b);

    DiagramItem *startItem = NULL;
    DiagramItem *endItem = NULL;

    foreach( QGraphicsItem *item, items() ) {
        if (item->type() == DiagramItem::Type) {
            DiagramItem * dia_obj = qgraphicsitem_cast<DiagramItem *>(item);
            if (!startItem) {
                if (dia_obj->GetSimItemID() == seg->startTrackPoint_id) {
                    startItem = dia_obj;
                }
            }
            if (!endItem) {
                if (dia_obj->GetSimItemID() == seg->endTrackPoint_id) {
                    endItem = dia_obj;
                }
            }
            if (startItem && endItem)
                break;
        }
    }

    if (!startItem ||  !endItem) {
        qWarning() << "Could not find parents for Segment " << seg->sim_id;
        return;
    }

    Arrow *arrow = new Arrow(startItem, endItem, 0);
    arrow->setColor(color);
    startItem->addArrow(arrow);
    endItem->addArrow(arrow);
    arrow->setZValue(-1000.0);
    addItem(arrow);
    arrow->updatePosition();
}

void DiagramScene::saveItems(QString & name)
{

    foreach( QGraphicsItem *item, items() ) {
        qDebug() << "type: " << item->type();
        if (item->type() == Arrow::Type) {
            // ******************************
            // *                            *
            // *     Segment (Arrow)        *
            // *                            *
            // ******************************
            Arrow *arrow = qgraphicsitem_cast<Arrow *>(item);
            QPointF startPos = arrow->getStartPos();
            //QPointF endPos = arrow->getEndPos();

            segment_t seg;
            memset(&seg, 0, sizeof(seg));
            strncpy(seg.name, "Segment", sizeof(seg.name) - 1);
            strncpy(seg.type, "Segment", sizeof(seg.type) - 1);            
            seg.sim_id = arrow->GetSimItemID();
            seg.pos_x = startPos.x();
            seg.pos_y = startPos.y();
            arrow->getColor(&seg.color_r, &seg.color_g, &seg.color_b);
            seg.startTrackPoint_id = arrow->startItem()->GetSimItemID();
            seg.endTrackPoint_id = arrow->endItem()->GetSimItemID();
            seg.startLightState = arrow->trafficLightStart;
            seg.endLightState = arrow->trafficLightStart;
            simInt_addSegment(&seg);


        }
        else if (item->type() == DiagramItem::Type) {
            // ******************************
            // *                            *
            // *     Item (TrackPoint...)   *
            // *                            *
            // ******************************

             DiagramItem * dia_obj = qgraphicsitem_cast<DiagramItem *>(item);
             qDebug() << "Item is a DiagramItem type: " << DiagramItem::Type << ", sim ID = " << dia_obj->GetSimItemID();
            if (dia_obj->diagramType() == DiagramItem::DiagramType::TrackPoint) {
                item_t it;
                memset(&it, 0, sizeof(it));
                strncpy(it.name, "TrackPoint", sizeof(it.name) - 1);
                strncpy(it.type, "TrackPoint", sizeof(it.type) - 1);

                it.sim_id = dia_obj->GetSimItemID();
                it.pos_x = dia_obj->pos().x(); 
                it.pos_y = dia_obj->pos().y();
                dia_obj->getColor(&it.color_r, &it.color_g, &it.color_b);
                int i = 0;

                foreach(auto a, dia_obj->arrows) {
                    if (i >= MAX_NUM_SEGS_PER_TRACKPOINT) {
                        qWarning() << "TrackPoint with too many segments, sim_id: " << it.sim_id;
                        break;
                    }
                    it.segment_id[i++] = a->GetSimItemID();
                    //qDebug() << "add segment_id[" << i-1 << "] = " << a->GetSimItemID();
                }
                simInt_addItem(&it);
            }
        }
        else if (item->type() == TEXT_ITEM_TYPE) {
            qDebug() << "Item is Text";
            DiagramTextItem * txt_obj = qgraphicsitem_cast<DiagramTextItem *>(item);
            text_t t;
            memset(&t, 0, sizeof(t));
            QString q_txt = txt_obj->toPlainText();
            if (q_txt.isEmpty())
                continue;
            strncpy(t.text, q_txt.toStdString().c_str(), sizeof(t.text) - 1);
            qDebug() << "Text: " << t.text;
            t.pos_x = txt_obj->pos().x(); 
            t.pos_y = txt_obj->pos().y();
            QColor color = txt_obj->defaultTextColor();
            color.getRgb(&t.color_r, &t.color_g, &t.color_b);
            QFont f = font();
            qDebug() << "Font size: " << f.pointSize() ;
            t.size = f.pointSize();
            QString family = f.family();
            qDebug() << "Family: " << family;
            strncpy(t.font_name, family.toStdString().c_str(), sizeof(t.font_name) - 1);
            simInt_addText(&t);
        }
        else {
            qDebug() << "Item is unknown: " << item->type();
        }

    }

    simInt_save(name.toStdString().c_str());
}

void DiagramScene::loadItems(QString &  name)
{
    qDebug() << "loadItems: " << name;
}

void DiagramScene::addText(text_t * txt)
{
    QPoint p(txt->pos_x , txt->pos_y);
    QColor color;
    QFont font;

    font.setPointSize(txt->size);
    font.setFamily(QString(txt->font_name));

    color.setRgb(txt->color_r, txt->color_g, txt->color_b);

    DiagramTextItem * textItem = new DiagramTextItem(0);
    //textItem->setFont(m_Font);
    textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
    textItem->setZValue(1000.0);
    connect(textItem, &DiagramTextItem::lostFocus,
            this, &DiagramScene::editorLostFocus);
    connect(textItem, &DiagramTextItem::selectedChange,
            this, &DiagramScene::itemSelected);
    addItem(textItem);
    textItem->setDefaultTextColor(color);
    textItem->setPlainText(QString(txt->text));

    textItem->setPos(p);
    textItem->setFont(font);
    emit textInserted(textItem);
}

void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
        return;

    //DiagramItem *item;
    switch (m_Mode) {
        case InsertItem:
            AddItem(m_ItemType,
                    m_ItemMenu,
                    mouseEvent->scenePos(),
                    m_ItemColor,
                    0);
            break;

        case InsertTrain:
            trainItem = new TrainItem(m_ItemMenu);
            trainItem->setZValue(1000.0);
            addItem(trainItem);
            trainItem->setPos(mouseEvent->scenePos());
            setMode(EditingTrain); //MoveItem;

            connect(trainItem, &TrainItem::routingEnded,
                    this, &DiagramScene::routingHasEnded);
            connect(trainItem, &TrainItem::routingStarted,
                    this, &DiagramScene::routingHasStarted);
            trains.append(trainItem);

            break;

        case EditingTrain:
            qDebug() << "Editing train";
            // {
            //     QList s = selectedItems();
            //     if (s.count() > 0) {
            //         qDebug() << "type: " << s[0]->type();

            //         if (s[0]->type() == Arrow::Type) {
            //             Arrow *arrow = qgraphicsitem_cast<Arrow *>(s[0]);
            //             arrow->showTrain = !arrow->showTrain;
            //         }

            //     }
            //     else {
            //         qDebug() << "no selection";
            //     }
            // }
            break;

//! [6] //! [7]
        case InsertLine:
            line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),
                                        mouseEvent->scenePos()));
            line->setPen(QPen(m_LineColor, 2));
            addItem(line);
            break;
//! [7] //! [8]
        case InsertText:
            textItem = new DiagramTextItem(0);
            textItem->setFont(m_Font);
            textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
            textItem->setZValue(1000.0);
            connect(textItem, &DiagramTextItem::lostFocus,
                    this, &DiagramScene::editorLostFocus);
            connect(textItem, &DiagramTextItem::selectedChange,
                    this, &DiagramScene::itemSelected);
            addItem(textItem);
            textItem->setDefaultTextColor(m_TextColor);
            textItem->setPos(mouseEvent->scenePos());
            emit textInserted(textItem);
//! [8] //! [9]
    default:
        ;
    }
    QGraphicsScene::mousePressEvent(mouseEvent);
}
//! [9]

//! [10]
void DiagramScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (m_Mode == InsertLine && line != nullptr) {
        QLineF newLine(line->line().p1(), mouseEvent->scenePos());
        line->setLine(newLine);
    } else if (m_Mode == MoveItem) {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}
//! [10]

//! [11]
void DiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (line != nullptr && m_Mode == InsertLine) {
        QList<QGraphicsItem *> startItems = items(line->line().p1());
        if (startItems.count() && startItems.first() == line)
            startItems.removeFirst();
        QList<QGraphicsItem *> endItems = items(line->line().p2());
        if (endItems.count() && endItems.first() == line)
            endItems.removeFirst();

        removeItem(line);
        delete line;
//! [11] //! [12]

        if (startItems.count() > 0 && endItems.count() > 0 &&
            startItems.first()->type() == DiagramItem::Type &&
            endItems.first()->type() == DiagramItem::Type &&
            startItems.first() != endItems.first()) {
            DiagramItem *startItem = qgraphicsitem_cast<DiagramItem *>(startItems.first());
            DiagramItem *endItem = qgraphicsitem_cast<DiagramItem *>(endItems.first());
            Arrow *arrow = new Arrow(startItem, endItem, 0);
            arrow->setColor(m_LineColor);
            startItem->addArrow(arrow);
            endItem->addArrow(arrow);
            arrow->setZValue(-1000.0);
            addItem(arrow);
            arrow->updatePosition();
        }
    }
//! [12] //! [13]
    line = nullptr;
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}
//! [13]

//! [14]
bool DiagramScene::isItemChange(int type) const
{
    const QList<QGraphicsItem *> items = selectedItems();
    const auto cb = [type](const QGraphicsItem *item) { return item->type() == type; };
    return std::find_if(items.begin(), items.end(), cb) != items.end();
}
//! [14]
