#include "internal.h"

#include <QPointF>

QPointF internal::getPointByStep(QPointF point, int step)
{
    qreal xV = round(point.x() / step) * step;
    qreal yV = round(point.y() / step) * step;

    point.setX(xV);
    point.setY(yV);

    return point;
}
