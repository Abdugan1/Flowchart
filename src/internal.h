#ifndef INTERNAL_H
#define INTERNAL_H

#include <QPointF>

namespace internal {

QPointF getPointByStep(QPointF point, int step)
{
    qreal xV = round(point.x() / step) * step;
    qreal yV = round(point.y() / step) * step;

    point.setX(xV);
    point.setY(yV);

    return point;
}

}

#endif // INTERNAL_H
