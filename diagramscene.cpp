// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "diagramscene.h"
#include "arrow.h"

#include <QGraphicsSceneMouseEvent>
#include <QTextCursor>
#include <QFile>


#include "rapidjson/document.h" 
#include "rapidjson/filewritestream.h" 
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"
#include <fstream> 
#include <iostream> 

//! [0]
DiagramScene::DiagramScene(QMenu *itemMenu, QObject *parent)
    : QGraphicsScene(parent)
{
    m_ItemMenu = itemMenu;
    m_Mode = MoveItem;
    m_ItemType = DiagramItem::Step;
    line = nullptr;
    textItem = nullptr;
    m_ItemColor = DEFAULT_ITEM_COLOR;
    m_TextColor = Qt::black;
    m_LineColor = Qt::black;
}
//! [0]

//! [1]
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

void DiagramScene::loadScene(QString data)
{
    // 3 TrackPoints:
    QPoint p1(2418 , 2346); 
    QPoint p2(2539 , 2513);
    QPoint p3(2275 , 2507);

    AddItem( DiagramItem::TrackPoint,
                            m_ItemMenu,
                            p1,
                            Qt::blue);

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
                            QColor color)
{
    DiagramItem *item;
    item = new DiagramItem(itemType, itemMenu);
    item->setBrush(color);
    addItem(item);
    item->setPos(pos);
    emit itemInserted(item);
}

void DiagramScene::saveItems(QString & name)
{
    char buffer[1024 * 4]; // 4K buffer
  
    std::FILE* f = fopen(name.toStdString().c_str(), "w");
    if (!f) { 
        qWarning() << "Could not open " << name;
        return;
    }

    rapidjson::Document d; 
    d.SetObject(); 

    // Add data to the JSON document 
    d.AddMember("name", "Geek", d.GetAllocator()); 
    d.AddMember("age", 30, d.GetAllocator());

    rapidjson::Value v;
    v.SetObject();
    v.AddMember("key", "value", d.GetAllocator());

    d.AddMember("nested", v, d.GetAllocator());



    foreach( QGraphicsItem *item, items() ) {
        if (item->type() == Arrow::Type) {
            Arrow *arrow = qgraphicsitem_cast<Arrow *>(item);
            QPointF startPos = arrow->getStartPos();
            QPointF endPos = arrow->getEndPos();
            qDebug() << "Item is an Arrow, sim ID = " << arrow->GetSimItemID();
            qDebug() << "  Start x: " << startPos.x() << ", y: " << startPos.y();
            qDebug() << "  End x: " << endPos.x() << ", y: " << endPos.y();
        }
        else if (item->type() == DiagramItem::Type) {
             DiagramItem * dia = qgraphicsitem_cast<DiagramItem *>(item);
             qDebug() << "Item is a DiagramItem type: " << dia->diagramItemType << ", sim ID = " << dia->GetSimItemID();
        }
        else if (item->type() == 65539) {
            qDebug() << "Item is Text";
        }
        else {
            qDebug() << "Item is unknown: " << item->type();
        }

    }

    // Write the JSON data to the file 
    rapidjson::FileWriteStream os(f, buffer, sizeof(buffer)); 
#ifdef COMPACT_JSON
    rapidjson::Writer<rapidjson::FileWriteStream> writer(os); 
#else
    rapidjson::PrettyWriter<rapidjson::FileWriteStream> writer(os);
#endif
    
    d.Accept(writer); 
 







}

void DiagramScene::loadItems(QString &  name)
{
    qDebug() << "loadItems: " << name;
}

//! [6]
// scenePos xp=2156 yp=2396
// m_ItemType = TrackPoint
void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
        return;

    DiagramItem *item;
    switch (m_Mode) {
        case InsertItem:
            AddItem(m_ItemType,
                    m_ItemMenu,
                    mouseEvent->scenePos(),
                    m_ItemColor);
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
            textItem = new DiagramTextItem();
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
            Arrow *arrow = new Arrow(startItem, endItem);
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
