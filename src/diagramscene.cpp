#include "diagramscene.h"
#include "diagramitem.h"
#include "diagramview.h"
#include "graphicsitemgroup.h"
#include "arrowmanager.h"

#include "node.h"
#include "constants.h"
#include "internal.h"

#include <QPainter>
#include <QDebug>
#include <QCursor>
#include <QFile>

DiagramScene::DiagramScene(QObject *parent)
    : QGraphicsScene(parent)
    , mode_(Normal)
{
    const int w = Constants::DiagramScene::A4Width  / Constants::DiagramScene::GridSize + 1;
    const int h = Constants::DiagramScene::A4Height / Constants::DiagramScene::GridSize + 1;
    qDebug() << "w:" << w;
    qDebug() << "h:" << h;
    nodes_.resize(w * h);
    qreal x = 0.0;
    qreal y = 0.0;
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            nodes_[i * w + j].pos.setX(x);
            nodes_[i * w + j].pos.setY(y);
            nodes_[i * w + j].parent = nullptr;
            nodes_[i * w + j].visited = false;

            x += Constants::DiagramScene::GridSize;
        }
        x = 0.0;
        y += Constants::DiagramScene::GridSize;
    }

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            if (y > 0)
                nodes_[y * w + x].neighbours.push_back(&nodes_[(y - 1) * w + (x + 0)]);
            if (y < w - 1)
                nodes_[y * w + x].neighbours.push_back(&nodes_[(y + 1) * w + (x + 0)]);
            if (x > 0)
                nodes_[y * w + x].neighbours.push_back(&nodes_[(y + 0) * w + (x - 1)]);
            if (x < w - 1)
                nodes_[y * w + x].neighbours.push_back(&nodes_[(y + 0) * w + (x + 1)]);
        }
    }

    QString str;

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            str += "(" + QString::number(nodes_[y * w + x].pos.x()) + ", " + QString::number(nodes_[y * w + x].pos.y()) + ") ";
        }
        str += "\n";
    }

    QFile file("nodes.txt");
    file.open(QFile::WriteOnly);
    QTextStream out(&file);
    out << str;
    file.close();
}

DiagramItem *DiagramScene::createDiagramItem(int diagramType)
{
    DiagramItem* diagramItem = new DiagramItem(DiagramItem::DiagramType(diagramType));

    connect(diagramItem, &DiagramItem::itemPositionChanged,
            this,        &DiagramScene::drawPositionLines);

    connect(diagramItem, &DiagramItem::itemReleased,
            this,        &DiagramScene::deleteAllPositionLines);

    connect(diagramItem->arrowManager(), &ArrowManager::handleClicked,
            this,                        &DiagramScene::onHandleClicked);

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

QList<DiagramItem *> DiagramScene::getDiagramItems(Qt::SortOrder order) const
{
    return internal::getDiagramItemsFromQGraphics(items(order));
}

QImage DiagramScene::toImage()
{
    QImage image(sceneRect().size().toSize(), QImage::Format_ARGB32);
    image.fill(Qt::transparent);

    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    render(&painter);

    return image;
}

void DiagramScene::clearAllSelection()
{
    QList<DiagramItem*> items = internal::getDiagramItemsFromQGraphics(this->items());
    if (!items.isEmpty()) {
        if (group_)
            destroyGraphicsItemGroup();
        else
            items.first()->setSelected(false);
    }
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
    QList<DiagramItem*> items = internal::getDiagramItemsFromQGraphics(this->items(Qt::AscendingOrder));
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
            internal::getDiagramItemsFromQGraphics(this->items(rect, Qt::IntersectsItemShape,
                                                               Qt::AscendingOrder));

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

void DiagramScene::onHandleClicked(const QPointF &mappedToScenePos)
{
    const int w = Constants::DiagramScene::A4Width  / Constants::DiagramScene::GridSize + 1;

    QPoint nodePos = mappedToScenePos.toPoint();
    nodePos = QPoint(nodePos.x() / Constants::DiagramScene::GridSize,
                     nodePos.y() / Constants::DiagramScene::GridSize);
    qDebug() << nodePos;

    if (mode_ == Normal) {
        mode_ = Line;
        nodeStart_ = &nodes_[nodePos.y() * w + nodePos.x()];
        qDebug() << "nodeStart:" << nodeStart_->pos;
    } else {
        nodeEnd_ = &nodes_[nodePos.y() * w + nodePos.x()];
        qDebug() << "nodeEnd:" << nodeEnd_->pos;
        solveAStar();

        QList<QLineF> lines;
        if (nodeEnd_ != nullptr) {
            Node* p = nodeEnd_;
            while (p->parent != nullptr) {
                QLineF line(p->pos.x(), p->pos.y(), p->parent->pos.x(), p->parent->pos.y());
                lines.append(line);
                p = p->parent;
            }
        }

        for (auto line : lines) {
            addLine(line);
        }

        mode_ = Normal;
    }
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

void DiagramScene::solveAStar()
{
    const int w = Constants::DiagramScene::A4Width  / Constants::DiagramScene::GridSize + 1;
    const int h = Constants::DiagramScene::A4Height / Constants::DiagramScene::GridSize + 1;

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            nodes_[y * w + x].visited = false;
            nodes_[y * w + x].globalGoal = INFINITY;
            nodes_[y * w + x].localGoal  = INFINITY;
            nodes_[y * w + x].parent = nullptr;
        }
    }

    auto distance = [](Node* a, Node* b)
    {
        return sqrtf((a->pos.x() - b->pos.x()) * (a->pos.x() - b->pos.x()) +
                     (a->pos.y() - b->pos.y()) * (a->pos.y() - b->pos.y()));
    };

    auto heuristic = [distance](Node* a, Node* b)
    {
        return distance(a, b);
    };

    Node* nodeCurrent = nodeStart_;
    nodeStart_->localGoal  = 0.0;
    nodeStart_->globalGoal = heuristic(nodeStart_, nodeEnd_);

    QList<Node*> notTestedNodes;
    notTestedNodes.append(nodeStart_);

    while (!notTestedNodes.isEmpty()) {
        /// It seems that it should work with this, but if it is
        /// not there, then this algorithm works as I need it
//        std::sort(notTestedNodes.begin(), notTestedNodes.end(), [](const Node* lhs, const Node* rhs) {return lhs->globalGoal < rhs->globalGoal;});

        while (!notTestedNodes.isEmpty() && notTestedNodes.front()->visited)
            notTestedNodes.pop_front();

        if (notTestedNodes.isEmpty())
            break;

        nodeCurrent = notTestedNodes.front();
        nodeCurrent->visited = true;

        for (auto nodeNeighbour : qAsConst(nodeCurrent->neighbours)) {
            if (!nodeNeighbour->visited)
                notTestedNodes.append(nodeNeighbour);

            qreal possiblyLowerGoal = nodeCurrent->localGoal + distance(nodeCurrent, nodeNeighbour);

            if (possiblyLowerGoal < nodeNeighbour->localGoal) {
                nodeNeighbour->parent = nodeCurrent;
                nodeNeighbour->localGoal = possiblyLowerGoal;
                nodeNeighbour->globalGoal = nodeNeighbour->localGoal + heuristic(nodeNeighbour, nodeEnd_);
            }
        }
    }

}
