#include "diagramscene.h"
#include "diagramitem.h"

#include <QPainter>
#include <QDebug>
#include <QStack>

DiagramScene::DiagramScene(QObject *parent)
    : QGraphicsScene(parent)
{
    QFont font("Times New Roman", 14);
    setFont(font);
}

void DiagramScene::onItemPositionChanging(const QPointF &oldPos, const QPointF &newPos)
{
    DiagramItem* senderItem = static_cast<DiagramItem*>(sender());

    QPoint oldCenter = oldPos.toPoint() + senderItem->boundingRect().center().toPoint();

    deleteAllLines(oldCenter);

    // Dont draw green dash line, if several items selected
    if (selectedItems().count() > 1)
        return;

    // Copying for convinience
    QList<QGraphicsItem*> allItems = this->items();
    QList<DiagramItem*> items;
    for (QGraphicsItem* i : qAsConst(allItems)) {
        DiagramItem* item = qgraphicsitem_cast<DiagramItem*>(i);
        if (item)
            items.append(item);
    }

    // Draw green dash line, if moving item is at level of others
    QPoint senderCenter = newPos.toPoint() + senderItem->boundingRect().center().toPoint();
    QPoint startX = senderCenter;
    QPoint endX = senderCenter;

    QPoint startY = senderCenter;
    QPoint endY = senderCenter;
    for (DiagramItem* item : qAsConst(items)) {
        QPoint itemCenter;
        if (item == senderItem)
            itemCenter = senderCenter;
        else
            itemCenter = getItemCenter(item);

        // Check "x" dimension
        if (itemCenter.x() == senderCenter.x()) {
            startX.setY(qMin(itemCenter.y(), startX.y()));
            endX.setY(qMax(itemCenter.y(), endX.y()));
        }

        // Check "y" dimension
        if (itemCenter.y() == senderCenter.y()) {
            startY.setX(qMin(itemCenter.x(), startY.x()));
            endY.setX(qMax(itemCenter.x(), endY.x()));
        }
    }

    if (startX.y() != senderCenter.y()) {
        drawGreenDashLine(QLineF(startX, endX));
    } else if (endX.y() != senderCenter.y()) {
        drawGreenDashLine(QLineF(endX, startX));
    }

    if (startY.x() != senderCenter.x()) {
        drawGreenDashLine(QLineF(startY, endY));
    } else if (endY.x() != senderCenter.x()) {
        drawGreenDashLine(QLineF(endY, startY));
    }
}

void DiagramScene::onItemReleased()
{
    DiagramItem* item = static_cast<DiagramItem*>(sender());
    QPoint center = item->pos().toPoint() + item->boundingRect().center().toPoint();
    deleteAllLines(center);
}

void DiagramScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    QPen pen;
    painter->setPen(pen);

    qreal left = int(rect.left() - (int(rect.left()) % GridSize));
    qreal top  = int(rect.top()  - (int(rect.top())  % GridSize));
    QVector<QPointF> points;
    for (qreal x = left; x < rect.right(); x += GridSize) {
        for (qreal y = top; y < rect.bottom(); y += GridSize) {
            points.append(QPointF(x, y));
        }
    }

    painter->drawPoints(points.data(), points.size());
}

void DiagramScene::deleteAllLines(const QPoint &point)
{
    bool hasLine = true;
    while (hasLine) {
        typedef  QGraphicsLineItem LineItem;
        LineItem* line = qgraphicsitem_cast<LineItem*>(itemAt(point, QTransform()));
        if (line)
            delete line;
        else
            hasLine = false;
    }
}

void DiagramScene::drawGreenDashLine(QLineF line)
{
    QGraphicsLineItem* lineItem = new QGraphicsLineItem(line);
    QPen pen;
    pen.setColor(Qt::green);
    pen.setStyle(Qt::DashLine);
    pen.setDashPattern(QVector<qreal>({10, 5}));
    lineItem->setPen(pen);
    addItem(lineItem);
}

QPoint DiagramScene::getItemCenter(const DiagramItem *item)
{
    return (item->pos().toPoint() + item->boundingRect().center().toPoint());
}
