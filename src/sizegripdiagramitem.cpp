#include "sizegripdiagramitem.h"
#include "sizehandleitem.h"
#include "diagramitem.h"
#include "handleitemappeararea.h"

#include "constants.h"
#include "internal.h"

#include <QDebug>

using PositionFlags = SizeHandleItem::PositionFlags;

SizeGripDiagramItem::SizeGripDiagramItem(DiagramItem *diagramItem, QObject *parent)
    : SizeGrip(diagramItem,
               diagramItem->pathBoundingRect(),
               QSizeF(Constants::DiagramItem::MinWidth,
                      Constants::DiagramItem::MinHeight),
               parent)
    , diagramItem_(diagramItem)
{
}

void SizeGripDiagramItem::resizeLogic()
{
    diagramItem_->setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);
    QRectF itemRect   = diagramItem_->pathBoundingRect();
    QRectF resizeRect = SizeGrip::resizeRect();

    qreal sx = resizeRect.width()  / itemRect.width();
    qreal sy = resizeRect.height() / itemRect.height();

    QPainterPath oldPath = diagramItem_->path();
    QPainterPath newPath = oldPath * QTransform::fromScale(sx, sy);

    QRectF pathRect = newPath.boundingRect();

    qreal dx = resizeRect.x() - pathRect.x();
    qreal dy = resizeRect.y() - pathRect.y();

    diagramItem_->moveBy(dx, dy);
    diagramItem_->setPath(newPath);
    diagramItem_->setSize(QSizeF(resizeRect.width(), resizeRect.height()));

    SizeGrip::setResizeRect(pathRect);
    diagramItem_->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}
