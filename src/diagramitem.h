#ifndef DIAGRAMITEM_H
#define DIAGRAMITEM_H

#include <QGraphicsItem>

#include "itemtypes.h"

class DiagramTextItem;
class SizeGrip;
class SizeGripDiagramItem;
class ArrowManager;
class ArrowItem;
class DiagramScene;

class DiagramItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

public:
    enum DiagramType {
        Terminal,
        Process,
        Desicion,
        InOut,
        ForLoop,
    };

    enum {Type = ItemTypes::DiagramItemType};

public:
    DiagramItem(DiagramType diagramType, QGraphicsItem* parent = nullptr);
    ~DiagramItem();
    DiagramType diagramType() const;

    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    QRectF boundingRect() const override;
    int type() const override {return Type;}
    QRectF pathBoundingRect() const;
    QPainterPath shape() const override;

    void resize(const QSizeF& size);
    void resize(qreal width, qreal height);

    static QPainterPath getDefaultShape(DiagramType diagramType);
    static QPixmap image(DiagramType diagramType);
    static QString getToolTip(int diagramType);

    QString text() const;
    void setText(const QString& text);

    QPainterPath path() const;
    void setPath(QPainterPath newPath);

    const QSizeF &size() const;
    void setSize(const QSizeF &newSize);

    ArrowManager *arrowManager() const;

    void addArrow(ArrowItem* arrow);
    void removeArrow(ArrowItem* arrow);
    void removeArrows();
    void updateArrows();

    SizeGrip *sizeGrip() const;

signals:
    void itemPositionChanged();
    void itemReleased();

public slots:
    void updateTextItemPosition();

private slots:
    void prepareGeomChange();

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
    void setTextCursor(int position);
    int getTextCursorPosition(const QPointF& clickedPos);

private:
    QPainterPath path_;
    QSizeF size_;
    DiagramType diagramType_;

    DiagramScene* scene_ = nullptr;

    bool textEditing_ = false;
    QPointF clickedPos_;

    DiagramTextItem*     textItem_     = nullptr;
    SizeGripDiagramItem* sizeGrip_     = nullptr;
    ArrowManager*        arrowManager_ = nullptr;
};

#endif // DIAGRAMITEM_H
