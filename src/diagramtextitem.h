#ifndef DIAGRAMTEXTITEM_H
#define DIAGRAMTEXTITEM_H

#include <QGraphicsTextItem>

#include "itemtypes.h"

class DiagramTextItem : public QGraphicsTextItem
{
    Q_OBJECT

public:
    enum {Type = ItemTypes::DiagramTextItemType};

public:
    DiagramTextItem(const QString& text, QGraphicsItem* parent = nullptr);
    DiagramTextItem(QGraphicsItem* parent = nullptr);
    void setAlignment(Qt::Alignment alignment);

    int type() const override;

public slots:
    void updateGeometry(int, int, int);
    void updateGeometry();
    void updatePosition();
    void setTextInteraction(bool on);

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event)  override;

private:
    void init();

private:
    Qt::Alignment alignment_;
};

#endif // DIAGRAMTEXTITEM_H
