#include "diagramscene.h"
#include "diagramitem.h"
#include "diagramview.h"
#include "pathresizer.h"
#include "graphicsitemgroup.h"
#include "internal.h"

#include <QPainter>
#include <QDebug>
#include <QCursor>

DiagramScene::DiagramScene(QObject *parent)
    : QGraphicsScene(parent)
{
}

QPointF DiagramScene::preventOutsideMove(QPointF newPosTopLeft, QGraphicsItem *item)
{
    QRectF itemBoundingRect = item->boundingRect();

    QPointF bottomRight;
    bottomRight.setX(itemBoundingRect.width()  + newPosTopLeft.x());
    bottomRight.setY(itemBoundingRect.height() + newPosTopLeft.y());

    return preventOutsideMove(newPosTopLeft, bottomRight);
}

QPointF DiagramScene::preventOutsideMove(QPointF newPosTopLeft, QPointF newPosBottomRight)
{
    QRectF sceneRect = this->sceneRect();

    if (!sceneRect.contains(newPosTopLeft)) {
        newPosTopLeft.setX(qMax(newPosTopLeft.x(), sceneRect.left()));
        newPosTopLeft.setY(qMax(newPosTopLeft.y(), sceneRect.top()));
    }
    if (!sceneRect.contains(newPosBottomRight)) {
        newPosTopLeft.setX(qMin(newPosBottomRight.x(), sceneRect.right())  -
                           (newPosBottomRight.x() - newPosTopLeft.x()));

        newPosTopLeft.setY(qMin(newPosBottomRight.y(), sceneRect.bottom()) -
                           (newPosBottomRight.y() - newPosTopLeft.y()));
    }
    return newPosTopLeft;
}

DiagramItem *DiagramScene::createDiagramItem(int diagramType)
{
    DiagramItem* diagramItem = new DiagramItem(DiagramItem::DiagramType(diagramType));

    connect(diagramItem, &DiagramItem::itemPositionChanged,
            this,        &DiagramScene::drawLevelLineWithItemOnSameAxis);

    connect(diagramItem, &DiagramItem::itemReleased,
            this,        &DiagramScene::deleteAllLevelLines);

    return diagramItem;
}

DiagramItem *DiagramScene::createDiagramItem(const ItemProperties &itemProperties)
{
    DiagramItem* diagramItem = createDiagramItem(itemProperties.diagramType());

    diagramItem->resize(itemProperties.size() );
    diagramItem->setText(itemProperties.text());
    diagramItem->setPos (itemProperties.pos() );

    return diagramItem;
}

QList<DiagramItem *> DiagramScene::getDiagramItems() const
{
    return internal::getDiagramItemsFromQGraphics(items());
}

QList<ItemProperties> DiagramScene::getDiagramItemsProperties(const QList<DiagramItem *> &diagramItems) const
{
    QList<ItemProperties> itemsProperties;
    itemsProperties.reserve(diagramItems.count());
    for (auto item : qAsConst(diagramItems)) {
        ItemProperties properties;
        obtainItemProperties(item, &properties);

        itemsProperties.append(properties);
    }
    return itemsProperties;
}

void DiagramScene::drawLevelLineWithItemOnSameAxis(const QPointF &pos)
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

    for (auto item : qAsConst(items)) {
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

void DiagramScene::deleteAllLevelLines()
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

void DiagramScene::clearScene()
{
    QList<QGraphicsItem*> items = this->items();
    for (auto item : qAsConst(items))
        delete item;
}

void DiagramScene::destroyGraphicsItemGroup()
{
    QList<DiagramItem*> groupItems
            = internal::getDiagramItemsFromQGraphics(group_->childItems());

    destroyItemGroup(group_);

    for (auto item : qAsConst(groupItems)) {
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

void DiagramScene::copySelectedItems()
{
    if (selectedItems().isEmpty())
        return;

    buffer_.reset();

    GraphicsItemGroup* group = qgraphicsitem_cast<GraphicsItemGroup*>(selectedItems().at(0));
    QList<DiagramItem*> tmp;

    if (group) {
        tmp = internal::getDiagramItemsFromQGraphics(group_->childItems());
        buffer_.setGroupCopied(true);
    } else {
        tmp = internal::getDiagramItemsFromQGraphics(selectedItems());
        buffer_.setGroupCopied(false);
    }

    buffer_.setCopiedItemsProperties(getDiagramItemsProperties(tmp));
}

void DiagramScene::pasteCopiedItems()
{
    if (buffer_.isEmpty())
        return;

    QPointF mousePosition = getMousePosMappedToScene();

    if (buffer_.groupCopied()) {
        QList<DiagramItem*> items;
        items.reserve(buffer_.copiedItemsProperties().count());

        for (const auto& properies : qAsConst(buffer_.copiedItemsProperties())) {
            DiagramItem* item = createDiagramItem(properies);
            item->setPos(properies.pos());

            items.append(item);
            addItem(item);
        }

        if (group_)
            destroyGraphicsItemGroup();
        createGraphicsItemGroup(items);

        QPointF pos = getPosThatItemCenterAtMousePos(mousePosition, group_);
        group_->setPos(pos);

    } else {
        ItemProperties properies = buffer_.copiedItemsProperties().at(0);
        DiagramItem* item = createDiagramItem(properies);

        QPointF pos = getPosThatItemCenterAtMousePos(mousePosition, item);

        addItem(item);
        item->setPos(pos);
    }
}

void DiagramScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    QPen pen;
    painter->setPen(pen);

    int left = int(rect.left() - (int(rect.left()) % GridSize));
    int top  = int(rect.top()  - (int(rect.top())  % GridSize));

    QVector<QPointF> points;
    for (int x = left; x < rect.right(); x += GridSize) {
        for (int y = top; y < rect.bottom(); y += GridSize) {
            points.append(QPointF(x, y));
        }
    }

    painter->drawPoints(points.data(), points.size());

    painter->drawRect(QRectF(0, 0, DiagramScene::Width, DiagramScene::Height));
}

void DiagramScene::deleteAllLines()
{
    QList<QGraphicsItem*> items = this->items();
    using LineItem = QGraphicsLineItem;
    for (auto item : qAsConst(items)) {
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
    for (auto item : diagramItems) {
        QPointF pos = item->pos();
        topLeft.setX(qMin(pos.x(), topLeft.x()));
        topLeft.setY(qMin(pos.y(), topLeft.y()));
    }

    group_ = new GraphicsItemGroup(topLeft);
    group_->setFlag(QGraphicsItem::ItemIsMovable);
    group_->setFlag(QGraphicsItem::ItemIsSelectable);
    group_->setFlag(QGraphicsItem::ItemSendsGeometryChanges);

    for (auto item : diagramItems) {
        item->setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);
        item->setSelected(true);
        group_->addToGroup(item);
    }
    addItem(group_);
    group_->setSelected(true);

    connect(group_, &GraphicsItemGroup::lostSelection,
            this,   &DiagramScene::destroyGraphicsItemGroup);
}

QPointF DiagramScene::getMousePosMappedToScene() const
{
    QGraphicsView* view = views().at(0);
    QPoint origin = view->mapFromGlobal(QCursor::pos());
    return view->mapToScene(origin);
}

QPointF DiagramScene::getPosThatItemCenterAtMousePos(const QPointF &mousePosition,
                                                     const QGraphicsItem *item) const
{
    QRectF itemRect = item->boundingRect();
    return QPointF(mousePosition.x() - (itemRect.left() + itemRect.width()  / 2),
                   mousePosition.y() - (itemRect.top()  + itemRect.height() / 2));
}
