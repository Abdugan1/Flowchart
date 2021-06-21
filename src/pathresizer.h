#ifndef PATHRESIZER_H
#define PATHRESIZER_H

#include "sizegripitem.h"

class PathResizer : public SizeGripItem::Resizer
{
public:
    void operator ()(QGraphicsItem *item, QRectF &rect) override;
};

#endif // PATHRESIZER_H
