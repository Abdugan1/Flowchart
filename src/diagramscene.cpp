#include "diagramscene.h"
#include "diagramitem.h"

#include <QPainter>
#include <QDebug>
#include <QStack>
#include <QGuiApplication>
#include <QCursor>

DiagramScene::DiagramScene(QObject *parent)
    : QGraphicsScene(parent)
{
    QFont font("Times New Roman", 14);
    setFont(font);
}

QPointF DiagramScene::preventOutsideMove(QPointF topLeft, QGraphicsItem *item)
{
    QRectF sceneRect = this->sceneRect();
    QRectF itemBoundingRect = item->boundingRect();

    QPointF bottomRight;
    bottomRight.setX(itemBoundingRect.width()  + topLeft.x());
    bottomRight.setY(itemBoundingRect.height() + topLeft.y());

    if (!sceneRect.contains(topLeft)) {
        topLeft.setX(qMax(topLeft.x(), sceneRect.left()));
        topLeft.setY(qMax(topLeft.y(), sceneRect.top()));
    }
    if (!sceneRect.contains(bottomRight)) {
        topLeft.setX(qMin(bottomRight.x(), sceneRect.right())  - (bottomRight.x() - topLeft.x()));
        topLeft.setY(qMin(bottomRight.y(), sceneRect.bottom()) - (bottomRight.y() - topLeft.y()));
    }
    return topLeft;
}

QPointF DiagramScene::getPositionWithStep(QPointF pos)
{
    int step = GridSize;
    qreal xV = round(pos.x() / step) * step;
    qreal yV = round(pos.y() / step) * step;

    pos.setX(xV);
    pos.setY(yV);

    return pos;
}

void DiagramScene::onItemPositionChanging(const QPointF &oldPos, const QPointF &newPos)
{
    if (!QGuiApplication::overrideCursor())
        QGuiApplication::setOverrideCursor(QCursor(Qt::SizeAllCursor));

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
        if (DiagramItem* item = qgraphicsitem_cast<DiagramItem*>(i))
            items.append(item);
    }

    QPoint senderCenter  = newPos.toPoint() + senderItem->boundingRect().center().toPoint();
    QPoint verticalBegin = senderCenter;
    QPoint verticalEnd   = senderCenter;

    QPoint horizontalBegin = senderCenter;
    QPoint horizontalEnd   = senderCenter;
    for (DiagramItem* item : qAsConst(items)) {
        QPoint itemCenter;
        if (item == senderItem)
            itemCenter = senderCenter;
        else
            itemCenter = getItemCenter(item);

        // Check "x" dimension
        if (itemCenter.x() == senderCenter.x()) {
            verticalBegin.setY(qMin(itemCenter.y(), verticalBegin.y()));
            verticalEnd.setY(qMax(itemCenter.y(), verticalEnd.y()));
        }

        // Check "y" dimension
        if (itemCenter.y() == senderCenter.y()) {
            horizontalBegin.setX(qMin(itemCenter.x(), horizontalBegin.x()));
            horizontalEnd.setX(qMax(itemCenter.x(), horizontalEnd.x()));
        }
    }

    if (verticalBegin.y() != senderCenter.y()) {
        drawGreenDashLine(QLineF(verticalBegin, verticalEnd));
    } else if (verticalEnd.y() != senderCenter.y()) {
        drawGreenDashLine(QLineF(verticalEnd, verticalBegin));
    }

    if (horizontalBegin.x() != senderCenter.x()) {
        drawGreenDashLine(QLineF(horizontalBegin, horizontalEnd));
    } else if (horizontalEnd.x() != senderCenter.x()) {
        drawGreenDashLine(QLineF(horizontalEnd, horizontalBegin));
    }
}

void DiagramScene::onItemReleased()
{
    DiagramItem* item = static_cast<DiagramItem*>(sender());
    QPoint center = item->pos().toPoint() + item->boundingRect().center().toPoint();
    deleteAllLines(center);
    QGuiApplication::restoreOverrideCursor();
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
