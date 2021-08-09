#ifndef SIZEGRIPITEM_H
#define SIZEGRIPITEM_H

#include <QGraphicsObject>

class DiagramItem;
class HandleItem;

class SizeGripItem : public QObject
{
    Q_OBJECT

public:
    SizeGripItem(DiagramItem* diagramItem, QObject* parent = nullptr);

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

public slots:
    void hideHandleItems();
    void showHandleItems();

private:
    void doResize();
    void resizeDiagramItem();
    void updateHandleItemsPositions();

private:

    DiagramItem* diagramItem_;
    QList<HandleItem*> handleItems_;
    QRectF rect_;
};

#endif // SIZEGRIPITEM_H
