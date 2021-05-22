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

    // Draw green dash line, if moving item is at level of others
    QList<QGraphicsItem*> items = this->items();
    QStack<QLineF> linesToDraw;
    for (QGraphicsItem* i : qAsConst(items)) {
        DiagramItem* item = qgraphicsitem_cast<DiagramItem*>(i);
        if (!item || item == senderItem)
            continue;

        QPoint center1 = newPos.toPoint() + senderItem->boundingRect().center().toPoint();
        QPoint center2 = getItemCenter(item);

        if (center1.x() == center2.x() || center1.y() == center2.y()) {
            linesToDraw.push(QLineF(center2, center1));
        }
    }

    for (QLineF line : linesToDraw)
        drawGreenDashLine(line);
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
