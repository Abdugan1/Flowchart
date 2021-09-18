#ifndef SCENEBOUNDARY_H
#define SCENEBOUNDARY_H

#include <QGraphicsItem>
#include <QObject>

#include "itemtypes.h"

class DiagramScene;
class SizeGrip;
class SizeGripSceneBoundary;
class ConfirmDialog;

class SceneBoundary : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

public:
    enum {Type = ItemTypes::SceneBoundaryType};

public:
    explicit SceneBoundary(DiagramScene* scene, const QRectF& rect, QGraphicsItem* parent = nullptr);
    explicit SceneBoundary(DiagramScene* scene, qreal x, qreal y, qreal w, qreal h, QGraphicsItem* parent = nullptr);

    ~SceneBoundary();

    SizeGrip *sizeGrip() const;

    ConfirmDialog *confirmDialog() const;

    int type() const override;

    const QRectF &rect() const;
    void setRect(const QRectF &newRect);

    QRectF boundingRect() const override;

    QPainterPath shape() const override;

    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

private slots:
    void showComfirmDialogOnMousePosition();

private:
    void init();
    void calculateAllGraphicsData();

private:
    QRectF boundingRect_;

    QRectF rect_;

    QRectF oldRect_;

    QLineF lines_[4];

    QPainterPath shape_;

    DiagramScene* scene_ = nullptr;

    SizeGripSceneBoundary* sizeGrip_ = nullptr;

    ConfirmDialog* confirmDialog_ = nullptr;

    QTimer* confirmDialogShowTimer_ = nullptr;
};

#endif // SCENEBOUNDARY_H
