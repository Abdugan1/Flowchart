#ifndef POSITIONLINE_H
#define POSITIONLINE_H

#include <QGraphicsLineItem>
#include <QtCore/qglobal.h>

class PositionLine : public QGraphicsLineItem
{
public:
    enum {Type = UserType + 3};

public:
    explicit PositionLine(const QLineF &line, QGraphicsItem *parent = nullptr);

    int type() const override;
};

#endif // POSITIONLINE_H
