#ifndef DIAGRAMITEM_H
#define DIAGRAMITEM_H

#include "diagramtextitem.h"

#include <QGraphicsPathItem>

class SizeGripItem;

class DiagramItem : public QObject, public QGraphicsPathItem
{
    Q_OBJECT

public:
    enum DiagramType {
        Terminal,
        Process,
        Desicion,
        InOut,
        ForLoop,
    };

    enum State {
        Other,
        TextEditing
    };

private:
    enum DefaultSize {
        Width  = 200,
        Height = 80
    };
    enum {
        DefaultPenWidth  = 1,
        SelectedPenWidth = 2
    };

public:
    DiagramItem(DiagramType diagramType, QGraphicsItem* parent = nullptr);
    DiagramType diagramType() const;

    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    QRectF boundingRect() const override;
    QRectF pathBoundingRect() const;

    void resize(const QSizeF& size);
    void resize(qreal width, qreal height);

    static QPainterPath getDefaultShape(DiagramType diagramType);
    static QPixmap image(DiagramType diagramType);
    static QString getToolTip(int diagramType);

    QString text() const;
    void setText(const QString& text);

signals:
    void itemPositionChanged(const QPointF& pos);
    void itemReleased();

public slots:
    void updateTextItemPosition();

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event)       override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event)        override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event)     override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

    void hoverMoveEvent(QGraphicsSceneHoverEvent *event)        override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event)       override;

    void keyPressEvent(QKeyEvent *event) override;

private:
    void enableTextEditing();
    void setTextCursorMappedToTextItem(const QPointF& clickPos);
private:
    DiagramType diagramType_;
    State state_;

    DiagramTextItem *textItem_;
    SizeGripItem    *sizeGripItem_;
};

#endif // DIAGRAMITEM_H
