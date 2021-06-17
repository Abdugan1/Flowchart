#ifndef POLYGONRESIZER_H
#define POLYGONRESIZER_H

#include "sizegripitem.h"

class PolygonResizer : public SizeGripItem::Resizer
{
public:
    void operator ()(QGraphicsItem *item, QRectF &rect) override;
};

#endif // POLYGONRESIZER_H
