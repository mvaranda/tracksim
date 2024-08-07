// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef DIAGRAMTEXTITEM_H
#define DIAGRAMTEXTITEM_H

#include <QGraphicsTextItem>
#include "simitemid.h"

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

#define TEXT_ITEM_TYPE (QGraphicsTextItem::UserType + 3)

//! [0]
class DiagramTextItem : public QGraphicsTextItem, public SimItemID
{
    Q_OBJECT

public:
    enum { Type = TEXT_ITEM_TYPE };

    DiagramTextItem(QGraphicsItem *parent = nullptr, int sim_id = 0);

    int type() const override { return Type; }

signals:
    void lostFocus(DiagramTextItem *item);
    void selectedChange(QGraphicsItem *item);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    void focusOutEvent(QFocusEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
};
//! [0]

#endif // DIAGRAMTEXTITEM_H
