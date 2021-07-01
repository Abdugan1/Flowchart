#include "diagramscene.h"
#include "diagramitem.h"
#include "graphicsitemgroup.h"

#include <QPainter>
#include <QDebug>
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

void DiagramScene::onItemPositionChanged(const QPointF &pos)
{
    deleteAllLines();

    // Dont draw green dash line, if several items selected
    if (selectedItems().count() > 1)
        return;

    DiagramItem* senderItem   = static_cast<DiagramItem*>(sender());
    QList<DiagramItem*> items = getDiagramItemsFromQGraphics(this->items());

    QPoint senderCenter  = pos.toPoint() + senderItem->boundingRect().center().toPoint();
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
        drawLevelLine(QLineF(verticalBegin, verticalEnd));
    } else if (verticalEnd.y() != senderCenter.y()) {
        drawLevelLine(QLineF(verticalEnd, verticalBegin));
    }

    if (horizontalBegin.x() != senderCenter.x()) {
        drawLevelLine(QLineF(horizontalBegin, horizontalEnd));
    } else if (horizontalEnd.x() != senderCenter.x()) {
        drawLevelLine(QLineF(horizontalEnd, horizontalBegin));
    }
}

void DiagramScene::onItemReleased()
{
    deleteAllLines();
}

void DiagramScene::selectAllItems()
{
    QList<DiagramItem*> items = getDiagramItemsFromQGraphics(this->items());
    if (group_)
        deleteGraphicsItemGroup();
    createGraphicsItemGroup(items);
}

void DiagramScene::deleteGraphicsItemGroup()
{
    QList<DiagramItem*> groupItems
            = getDiagramItemsFromQGraphics(group_->childItems());

    destroyItemGroup(group_);

    for (auto* item : groupItems)
        item->setSelected(false);

    group_ = nullptr;
}

void DiagramScene::makeGroupSelectedItems()
{
    QList<DiagramItem*> selectedItems = getDiagramItemsFromQGraphics(this->selectedItems());
    if (selectedItems.count() > 0)
        createGraphicsItemGroup(selectedItems);
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

QList<DiagramItem *> DiagramScene::getDiagramItemsFromQGraphics(const QList<QGraphicsItem *> items)
{
    QList<DiagramItem*> diagramItems;

    for (QGraphicsItem* i : qAsConst(items)) {
        if (DiagramItem* item = qgraphicsitem_cast<DiagramItem*>(i))
            diagramItems.append(item);
    }

    return diagramItems;
}

void DiagramScene::deleteAllLines()
{
    QList<QGraphicsItem*> items = this->items();
    using LineItem = QGraphicsLineItem;
    for (auto* item : items) {
        LineItem* line = qgraphicsitem_cast<LineItem*>(item);
        if (line)
            delete line;
    }
}

void DiagramScene::drawLevelLine(const QLineF& line)
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

void DiagramScene::createGraphicsItemGroup(QList<DiagramItem *>& diagramItems)
{
    group_ = new GraphicsItemGroup;
    group_->setFlag(QGraphicsItem::ItemIsMovable);
    group_->setFlag(QGraphicsItem::ItemIsSelectable);
    group_->setFlag(QGraphicsItem::ItemSendsGeometryChanges);

    for (auto* item : diagramItems) {
        item->setSelected(true);
        group_->addToGroup(item);
    }
    addItem(group_);
    group_->setSelected(true);

    connect(group_, &GraphicsItemGroup::lostSelection,
            this, &DiagramScene::deleteGraphicsItemGroup);
}
