#include "positionline.h"

#include <QPen>

PositionLine::PositionLine(const QLineF &line, QGraphicsItem *parent)
    : QGraphicsLineItem(line, parent)
{
    QPen pen;
    pen.setColor(Qt::green);
    pen.setStyle(Qt::DashLine);
    pen.setDashPattern(QVector<qreal>({10, 5}));

    setPen(pen);
}

int PositionLine::type() const
{
    return Type;
}
