#include "diagramscene.h"
#include "diagramitem.h"
#include "diagramview.h"
#include "graphicsitemgroup.h"

#include "constants.h"
#include "internal.h"

#include <QPainter>
#include <QDebug>
#include <QCursor>

DiagramScene::DiagramScene(QObject *parent)
    : QGraphicsScene(parent)
{
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
            this,        &DiagramScene::drawPositionLines);

    connect(diagramItem, &DiagramItem::itemReleased,
            this,        &DiagramScene::deleteAllPositionLines);

    return diagramItem;
}

DiagramItem *DiagramScene::createDiagramItem(const ItemProperties &itemProperties)
{
    DiagramItem* diagramItem = createDiagramItem(itemProperties.diagramType());

    diagramItem->resize(itemProperties.size() );
    diagramItem->setText(itemProperties.text());
    setItemPosWithoutDrawingPositionLines(diagramItem, itemProperties.pos());

    return diagramItem;
}

QList<DiagramItem *> DiagramScene::getDiagramItems() const
{
    return internal::getDiagramItemsFromQGraphics(items());
}

void DiagramScene::drawPositionLines()
{
    if (!drawPositionLines_)
        return;

    deleteAllLines();

    // Dont draw green dash line, if several items selected
    if (selectedItems().count() > 1)
        return;

    DiagramItem* senderItem   = static_cast<DiagramItem*>(sender());
    QList<DiagramItem*> items = internal::getDiagramItemsFromQGraphics(this->items());

    QPoint senderCenter  = getItemCenter(senderItem);
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

void DiagramScene::deleteAllPositionLines()
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

    for (auto item : qAsConst(groupItems)) {
        item->setSelected(false);
        item->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    }

    group_ = nullptr;
}

void DiagramScene::selectAndMakeGroup(const QRectF &rect)
{    
    QList<DiagramItem*> selectedItems =
            internal::getDiagramItemsFromQGraphics(this->items(rect));

    if (selectedItems.count() > 0)
        createGraphicsItemGroup(selectedItems);
}

void DiagramScene::deleteSelectedItems()
{
    deleteItems(selectedItems());
}

void DiagramScene::deleteItems(const QList<QGraphicsItem *> &items)
{
    if (items.isEmpty())
        return;

    QGraphicsItem* itemToDelete = items.at(0);

    // item could be a group, or it could be a single item
    if (qgraphicsitem_cast<GraphicsItemGroup*>(itemToDelete)) {
        delete group_;
        group_ = nullptr;
    } else {
        delete itemToDelete;
    }
}

void DiagramScene::copySelectedItems()
{
    copyItems(selectedItems());
}

void DiagramScene::copyItems(const QList<QGraphicsItem *> &items)
{
    if (items.isEmpty())
        return;

    buffer_.reset();

    GraphicsItemGroup* group = qgraphicsitem_cast<GraphicsItemGroup*>(items.at(0));
    QList<DiagramItem*> tmp;

    if (group) {
        tmp = internal::getDiagramItemsFromQGraphics(group_->childItems());
        buffer_.setGroupCopied(true);
    } else {
        tmp = internal::getDiagramItemsFromQGraphics(items);
        buffer_.setGroupCopied(false);
    }

    buffer_.setCopiedItemsProperties(internal::getDiagramItemsProperties(tmp));
}

void DiagramScene::pasteItemsToMousePos()
{
    QPointF mousePosition = getMousePosMappedToScene();
    pasteItems(mousePosition);
}

void DiagramScene::pasteItems(const QPointF &posToPaste)
{
    if (buffer_.isEmpty())
        return;

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

        QPointF pos = getPosThatItemCenterAtMousePos(posToPaste, group_);
        group_->setPos(pos);

    } else {
        ItemProperties properies = buffer_.copiedItemsProperties().at(0);
        DiagramItem* item = createDiagramItem(properies);

        QPointF pos = getPosThatItemCenterAtMousePos(posToPaste, item);

        addItem(item);
        setItemPosWithoutDrawingPositionLines(item, pos);
    }
}

void DiagramScene::clearScene()
{
    if (group_) {
        delete group_;
        group_ = nullptr;
    }
    clear();
}

void DiagramScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    QPen pen;
    painter->setPen(pen);

    int left = int(rect.left() - (int(rect.left()) % Constants::DiagramScene::GridSize));
    int top  = int(rect.top()  - (int(rect.top())  % Constants::DiagramScene::GridSize));

    QVector<QPointF> points;
    for (int x = left; x < rect.right(); x += Constants::DiagramScene::GridSize) {
        for (int y = top; y < rect.bottom(); y += Constants::DiagramScene::GridSize) {
            points.append(QPointF(x, y));
        }
    }

    painter->drawPoints(points.data(), points.size());

    painter->drawRect(QRectF(0, 0, Constants::DiagramScene::A4Width, Constants::DiagramScene::A4Height));
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

QPoint DiagramScene::getItemCenter(const DiagramItem *item) const
{
    return (item->pos().toPoint() + item->pathBoundingRect().center().toPoint());
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

void DiagramScene::setItemPosWithoutDrawingPositionLines(DiagramItem *item, const QPointF &pos)
{
    drawPositionLines_ = false;
    item->setPos(pos);
    drawPositionLines_ = true;
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
