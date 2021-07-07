#include "diagramscene.h"
#include "diagramitem.h"
#include "graphicsitemgroup.h"
#include "internal.h"

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
    QRectF itemBoundingRect = item->boundingRect();

    QPointF bottomRight;
    bottomRight.setX(itemBoundingRect.width()  + topLeft.x());
    bottomRight.setY(itemBoundingRect.height() + topLeft.y());

    return preventOutsideMove(topLeft, bottomRight);
}

QPointF DiagramScene::preventOutsideMove(QPointF topLeft, QPointF bottomRight)
{
    QRectF sceneRect = this->sceneRect();

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
    QList<DiagramItem*> items = internal::getDiagramItemsFromQGraphics(this->items());

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
    QList<DiagramItem*> items = internal::getDiagramItemsFromQGraphics(this->items());
    if (!items.isEmpty()) {
        if (group_)
            destroyGraphicsItemGroup();
        createGraphicsItemGroup(items);
    }
}

void DiagramScene::destroyGraphicsItemGroup()
{
    QList<DiagramItem*> groupItems
            = internal::getDiagramItemsFromQGraphics(group_->childItems());

    destroyItemGroup(group_);

    for (auto* item : groupItems) {
        item->setSelected(false);
        item->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    }

    group_ = nullptr;
}

void DiagramScene::makeGroupOfSelectedItems()
{
    QList<DiagramItem*> selectedItems =
            internal::getDiagramItemsFromQGraphics(this->selectedItems());

    if (selectedItems.count() > 0)
        createGraphicsItemGroup(selectedItems);
}

void DiagramScene::deleteSelectedItems()
{
    if (!selectedItems().isEmpty()) {
        QGraphicsItem* selectedItem = selectedItems().at(0);

        // item could be a group, or it could be a single item
        if (qgraphicsitem_cast<GraphicsItemGroup*>(selectedItem)) {
            delete group_;
            group_ = nullptr;
        } else {
            delete selectedItem;
        }
    }
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
    QPointF topLeft = diagramItems.at(0)->pos();
    for (auto* item : diagramItems) {
        QPointF pos = item->pos();
        topLeft.setX(qMin(pos.x(), topLeft.x()));
        topLeft.setY(qMin(pos.y(), topLeft.y()));
    }

    group_ = new GraphicsItemGroup(topLeft);
    group_->setFlag(QGraphicsItem::ItemIsMovable);
    group_->setFlag(QGraphicsItem::ItemIsSelectable);
    group_->setFlag(QGraphicsItem::ItemSendsGeometryChanges);

    for (auto* item : diagramItems) {
        item->setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);
        item->setSelected(true);
        group_->addToGroup(item);
    }
    addItem(group_);
    group_->setSelected(true);

    connect(group_, &GraphicsItemGroup::lostSelection,
            this, &DiagramScene::destroyGraphicsItemGroup);
}
