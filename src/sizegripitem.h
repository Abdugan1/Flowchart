#ifndef SIZEGRIPITEM_H
#define SIZEGRIPITEM_H

#include <QGraphicsObject>

class HandleItem;
class HandleItemAppearArea;

class SizeGripItem : public QGraphicsObject
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

public:
    class Resizer
    {
    public:
        virtual void operator()(QGraphicsItem* item,
                                QRectF& rect) = 0;
        virtual ~Resizer() {}
    };

public:
    SizeGripItem(Resizer* resizer, QGraphicsItem* parent);
    virtual ~SizeGripItem();

    QRectF boundingRect() const override;
    void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget) override;

    void setTopLeft(const QPointF& pos);
    void setTop(qreal y);
    void setTopRight(const QPointF& pos);
    void setRight(qreal x);
    void setBottomRight(const QPointF& pos);
    void setBottom(qreal y);
    void setBottomLeft(const QPointF& pos);
    void setLeft(qreal x);

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

signals:
    void resizeBeenMade();

public slots:
    void hideHandleItems();
    void showHandleItems();

private:
    void doResize();
    void updateHandleItemsPositions();

private:
    enum {
        MinWidth  = 80,
        MinHeight = 60
    };

    QList<HandleItem*> handleItems_;
    QList<HandleItemAppearArea*> appearAreas_;
    QRectF rect_;
    Resizer* resizer_;
};

#endif // SIZEGRIPITEM_H
