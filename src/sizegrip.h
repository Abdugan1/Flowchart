#ifndef SIZEGRIP_H
#define SIZEGRIP_H

#include <QtCore/qglobal.h>

#include "handlemanager.h"

class SizeGrip : public HandleManager
{
    Q_OBJECT

public:
    SizeGrip(QGraphicsItem* item, const QRectF& resizeRect, const QSizeF& minGripSize, QObject* parent = nullptr);

    void setTopLeft(const QPointF& pos);
    void setTop(qreal y);
    void setTopRight(const QPointF& pos);
    void setRight(qreal x);
    void setBottomRight(const QPointF& pos);
    void setBottom(qreal y);
    void setBottomLeft(const QPointF& pos);
    void setLeft(qreal x);

    void resize(const QRectF& rect);

    const QSizeF &minGripSize() const;
    void setMinGripSize(const QSizeF &newMinGripSize);

    const QRectF &maxGripArea() const;
    void setMaxGripArea(const QRectF &newMaxGripArea);

signals:
    void getReadyToResize();
    void resizeBeenMade();

protected:
    const QRectF &resizeRect() const;
    void setResizeRect(const QRectF &newRect);

    virtual void resizeLogic() = 0;

private:
    void doResize();
    void updateHandleItemsPositions();

private:
    QRectF resizeRect_;

    QSizeF minGripSize_;
    QRectF maxGripArea_;
};

#endif // SIZEGRIP_H
