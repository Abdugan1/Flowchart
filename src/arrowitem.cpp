#include "arrowitem.h"
#include "arrowhandleitem.h"
#include "diagramitem.h"

#include "constants.h"

#include <QDebug>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

ArrowItem::ArrowItem(QGraphicsItem *parent)
    : QGraphicsPathItem(parent)
{
    setZValue(-1);
    setAcceptHoverEvents(true);
    setFlag(ItemIsSelectable);
}

int ArrowItem::type() const
{
    return Type;
}

void ArrowItem::setConnectionPath(const QList<QLineF> &lines)
{
    lines_ = lines;
    calculateShape();
    QPainterPath pathShape;
    pathShape.moveTo(lines.first().p1());
    for (auto line : lines) {
        pathShape.lineTo(line.p2());
    }

    QGraphicsPathItem::setPath(pathShape);
}

void ArrowItem::setStartItem(ArrowHandleItem *handle, DiagramItem *item)
{
    startItem_.item   = item;
    startItem_.handle = handle;
}

DiagramItem *ArrowItem::startItem() const
{
    return startItem_.item;
}

void ArrowItem::setEndItem(ArrowHandleItem *handle, DiagramItem *item)
{
    endItem_.item   = item;
    endItem_.handle = handle;
}

DiagramItem *ArrowItem::endItem() const
{
    return endItem_.item;
}

QPoint ArrowItem::startPoint() const
{
    return startItem_.handle->scenePos().toPoint();
}

QPoint ArrowItem::endPoint() const
{
    return endItem_.handle->scenePos().toPoint();
}

void ArrowItem::updateConnectionPath()
{
    PositionFlags startHandleFlag = startItem_.handle->positionFlags();
    PositionFlags endHandleFlag   = endItem_.handle->positionFlags();

    QPoint startPos = startPoint();
    QPoint endPos   = endPoint();

    QPointF startFinish = getFinishConnectPoint(startItem(), startHandleFlag);
    QPointF endFinish   = getFinishConnectPoint(endItem(),   endHandleFlag);

    QList<QLineF> connectionLines;
    if (isCaseOfReverseConnection(startHandleFlag, endHandleFlag, startPos, endPos)) {
        connectionLines = getReverseConnection(startPos, endPos, startFinish, endFinish);
    } else {
        connectionLines = getDefaultConnection(startPos, endPos, startFinish, endFinish);
    }

    setConnectionPath(connectionLines);
    updateArrowHead(endFinish, endHandleFlag);
}

void ArrowItem::paint(QPainter *painter,
                      const QStyleOptionGraphicsItem *option,
                      QWidget *widget)
{
    Q_UNUSED(widget)

    QColor color;
    if (option->state & QStyle::State_Selected) {
        color = Qt::green;
    } else if (option->state & QStyle::State_MouseOver) {
        color = QColor(52, 201, 36);
    }

    painter->setPen(QPen(color));
    painter->drawPath(path());

    painter->setBrush(QBrush(color));
    painter->drawPolygon(arrowHead_);
}

QPainterPath ArrowItem::shape() const
{
    return shape_;
}

void ArrowItem::calculateShape()
{
    shape_.clear();
    shape_.setFillRule(Qt::WindingFill);
    for (auto line : qAsConst(lines_)) {
        QPointF topLeft(qMin(line.p1().x(), line.p2().x()) - 5,
                        qMin(line.p1().y(), line.p2().y()) - 5);
        QPointF bottomRight(qMax(line.p1().x(), line.p2().x()) + 5,
                            qMax(line.p1().y(), line.p2().y()) + 5);
        QRectF rect(topLeft, bottomRight);
        shape_.addRect(rect);
    }
}

void ArrowItem::updateArrowHead(const QPointF &endPoint, PositionFlags pf)
{
    const qreal w = 5.0;
    const qreal h = 7.0;
    arrowHead_.clear();

    switch (pf) {
    case Top:
        arrowHead_ << QPointF(endPoint.x() - w / 2, endPoint.y() - h)
                   << QPointF(endPoint.x() + w / 2, endPoint.y() - h)
                   << endPoint;
        break;
    case Left:
        arrowHead_ << QPointF(endPoint.x() - h, endPoint.y() + w / 2)
                   << QPointF(endPoint.x() - h, endPoint.y() - w / 2)
                   << endPoint;
        break;
    case Right:
        arrowHead_ << QPointF(endPoint.x() + h, endPoint.y() - w / 2)
                   << QPointF(endPoint.x() + h, endPoint.y() + w / 2)
                   << endPoint;
        break;
    case Bottom:
        arrowHead_ << QPointF(endPoint.x() + w / 2, endPoint.y() + h)
                   << QPointF(endPoint.x() - w / 2, endPoint.y() + h)
                   << endPoint;
        break;

    default: break;
    }
}

QList<QLineF> ArrowItem::getMainConnectionLines(const QPoint &startPoint, const QPoint &endPoint)
{
    QPoint p(startPoint.x(), endPoint.y());
    QLineF l1(startPoint, p);
    QLineF l2(p, endPoint);
    return {l1, l2};
}

QList<QLineF> ArrowItem::getDefaultConnection(const QPoint& startPos, const QPoint& endPos,
                                              const QPointF& startFinish, const QPointF& endFinish)
{
    return getConnectionLines(startPos, endPos, startFinish, endFinish);
}

QList<QLineF> ArrowItem::getReverseConnection(const QPoint &startPos, const QPoint &endPos,
                                  const QPointF &startFinish, const QPointF &endFinish)
{
    return getConnectionLines(endPos, startPos, endFinish, startFinish);
}

QList<QLineF> ArrowItem::getConnectionLines(const QPoint &startPos, const QPoint &endPos,
                                            const QPointF &startFinish, const QPointF &endFinish)
{
    QList<QLineF> connectionLines = getMainConnectionLines(startPos, endPos);
    setFinishConnections(connectionLines, startFinish, endFinish);
    return connectionLines;
}

void ArrowItem::setFinishConnections(QList<QLineF> &lines, const QPointF &startFinish, const QPointF &endFinish)
{
    QLineF connectionLineBegin(startFinish, lines.first().p1());
    QLineF connectionLineEnd(lines.last().p2(), endFinish);
    lines.prepend(connectionLineBegin);
    lines.append(connectionLineEnd);
}

bool ArrowItem::isCaseOfReverseConnection(PositionFlags startHandleFlag, PositionFlags endHandleFlag,
                                          const QPointF &startPos, const QPointF &endPos)
{
    if (startHandleFlag == Top && endHandleFlag == Top && startPos.y() < endPos.y()) {
        /// Top
        return true;

    } else if (startHandleFlag == Left && startPos.x() > endPos.x()) {
        /// Left
        return true;

    } else if ((startHandleFlag == Right || endHandleFlag  == Right) && endPos.x() > startPos.x()) {
        /// Right -> not implemented!
        return true;

    } else if (startHandleFlag == Bottom
               && (endHandleFlag == Top || endHandleFlag == Bottom || endHandleFlag == Left)
               && startPos.y() > endPos.y()) {
        /// Bottom
        return true;

    }
    return false;
}

QPointF ArrowItem::leftSideConnectionPointInOutDiagramItem(DiagramItem *diagramItem)
{
    QSizeF size = diagramItem->shape().boundingRect().size();
    int w = size.width();
    int h = size.height();
    QRectF rect(0, 0, w / 4, h);
    return diagramItem->scenePos() + rect.center();
}

QPointF ArrowItem::rightSideConnectionPointInOutDiagramItem(DiagramItem *diagramItem)
{
    QSizeF size = diagramItem->shape().boundingRect().size();
    int w = size.width();
    int h = size.height();
    QRectF rect(w - w / 4, 0, w / 4, h);
    return diagramItem->scenePos() + rect.center();
}

QPointF ArrowItem::getFinishConnectPoint(DiagramItem *diagramItem, PositionFlags handlePosFlag)
{
    QPointF finishPoint;
    const QRectF rect = diagramItem->shape().boundingRect();

    if (diagramItem->diagramType() == DiagramItem::InOut) {
        if (handlePosFlag == Left) {
            return leftSideConnectionPointInOutDiagramItem(diagramItem);
        } else if (handlePosFlag == Right) {
            return rightSideConnectionPointInOutDiagramItem(diagramItem);
        }
    }

    switch (handlePosFlag) {
    case Top:
        finishPoint = QPointF(diagramItem->scenePos().x() + rect.width() / 2,
                              diagramItem->scenePos().y());
        break;
    case Left:
        finishPoint = QPointF(diagramItem->scenePos().x(),
                              diagramItem->scenePos().y() + rect.height() / 2);
        break;
    case Right:
        finishPoint = QPointF(diagramItem->scenePos().x() + rect.right(),
                              diagramItem->scenePos().y() + rect.height() / 2);
        break;
    case Bottom:
        finishPoint = QPointF(diagramItem->scenePos().x() + rect.width() / 2,
                              diagramItem->scenePos().y() + rect.bottom());
        break;

    default:
        break;
    }

    return finishPoint;
}
