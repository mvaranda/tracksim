// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef DIAGRAMSCENE_H
#define DIAGRAMSCENE_H

#include "common.h"
#include "diagramitem.h"
#include "trainitem.h"
#include "diagramtextitem.h"


#include <QGraphicsScene>
#include "python_int.h"

#define DEFAULT_ITEM_COLOR Qt::yellow

typedef enum {
    CMD_SEGMENT_TRAIN_UNPRESENT,
    CMD_SEGMENT_TRAIN_PRESENT,
    CMD_SEGMENT_LIGHT_GREEN,
    CMD_SEGMENT_LIGHT_RED,
    CMD_SEGMENT_INT_RED
} cmd_for_segment_t;

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
class QMenu;
class QPointF;
class QGraphicsLineItem;
class QFont;
class QGraphicsTextItem;
class QColor;
QT_END_NAMESPACE

//! [0]
class DiagramScene : public QGraphicsScene
{
    Q_OBJECT

public:
    enum Mode { InsertItem, 
                InsertLine, 
                InsertText, 
                MoveItem, 
                InsertTrain,
                EditingTrain,
                Simulating };

    explicit DiagramScene(QMenu *itemMenu, QMenu *trainMenu, QObject *parent = nullptr);
    QFont font() const { return m_Font; }
    QColor textColor() const { return m_TextColor; }
    QColor itemColor() const { return m_ItemColor; }
    QColor lineColor() const { return m_LineColor; }
    void setLineColor(const QColor &color);
    void setTextColor(const QColor &color);
    void setItemColor(const QColor &color);
    void setFont(const QFont &font);
    void AddItem(   DiagramItem::DiagramType itemType,
                    QMenu * itemMenu,
                    QPointF pos,
                    QColor color,
                    int sim_id = 0);
    void AddSegment(segment_t * seg);
    void addText(text_t * txt);
    void addTrain(train_t * train);
    void loadScene(QString data);
    void sendItemsToSim();
    void loadItems(QString &  name);
    void cmd_for_segment(   cmd_for_segment_t cmd, 
                            int id, 
                            int par1 = 0, 
                            int par2 = 0,
                            int par3 = 0, 
                            int par4 = 0);
    void reset_railway();


public slots:
    void setMode(Mode mode);
    void setItemType(DiagramItem::DiagramType type);
    void editorLostFocus(DiagramTextItem *item);
    void routingHasStarted(QGraphicsItem *item);
    void routingHasEnded(QGraphicsItem *item);

signals:
    void itemInserted(DiagramItem *item);
    void textInserted(QGraphicsTextItem *item);
    void itemSelected(QGraphicsItem *item);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;

public:
    QMenu *m_ItemMenu;
    QMenu *m_TrainMenu;

private:
    bool isItemChange(int type) const;

    DiagramItem::DiagramType m_ItemType;

    Mode m_Mode;
    bool leftButtonDown;
    QPointF startPoint;
    QGraphicsLineItem *line;
    QFont m_Font;
    DiagramTextItem *textItem;
    TrainItem *trainItem;
    QColor m_TextColor;
    QColor m_ItemColor;
    QColor m_LineColor;
    QList <TrainItem *> trains;
};
//! [0]

#endif // DIAGRAMSCENE_H
