#ifndef SIZEGRIPITEM_H
#define SIZEGRIPITEM_H

#include <QGraphicsItem>
#include <QGraphicsRectItem>

class HandleItem;

class SizeGripItem : public QObject, public QGraphicsItem
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
    SizeGripItem(Resizer* resizer = 0, QGraphicsItem* parent = 0);
    virtual ~SizeGripItem();
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter* painter,
                       const QStyleOptionGraphicsItem* option,
                       QWidget* widget = 0);
    void setTopLeft(const QPointF& pos);
    void setTop(qreal y);
    void setTopRight(const QPointF& pos);
    void setRight(qreal x);
    void setBottomRight(const QPointF& pos);
    void setBottom(qreal y);
    void setBottomLeft(const QPointF& pos);
    void setLeft(qreal x);

signals:
    void itemResized();

private:
    void doResize();
    void updateHandleItemPositions();

    QList<HandleItem*> handleItems_;
    QRectF rect_;
    Resizer* resizer_;
};

#endif // SIZEGRIPITEM_H
