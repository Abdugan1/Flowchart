#ifndef SIZEGRIP_H
#define SIZEGRIP_H

#include "handlemanager.h"

#include <QRectF>

class DiagramItem;
class SizeHandleItem;
class HandleItemAppearArea;

class SizeGrip : public HandleManager
{
    Q_OBJECT

public:
    SizeGrip(DiagramItem* diagramItem, QObject* parent = nullptr);

    void setTopLeft(const QPointF& pos);
    void setTop(qreal y);
    void setTopRight(const QPointF& pos);
    void setRight(qreal x);
    void setBottomRight(const QPointF& pos);
    void setBottom(qreal y);
    void setBottomLeft(const QPointF& pos);
    void setLeft(qreal x);

    void setRect(const QRectF& rect);

    DiagramItem *diagramItem() const;

signals:
    void resizeBeenMade();

private:
    void doResize();
    void resizeDiagramItem();
    void updateHandleItemsPositions();

private:
    QRectF rect_;
};

#endif // SIZEGRIP_H
