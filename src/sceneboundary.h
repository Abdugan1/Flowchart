#ifndef SCENEBOUNDARY_H
#define SCENEBOUNDARY_H

#include <QGraphicsRectItem>
#include <QObject>

#include "itemtypes.h"

class DiagramScene;
class SizeGrip;
class SizeGripSceneBoundary;
class ConfirmDialog;

class SceneBoundary : public QObject, public QGraphicsRectItem
{
    Q_OBJECT

public:
    enum {Type = ItemTypes::SceneBoundaryType};

public:
    explicit SceneBoundary(DiagramScene* scene, const QRectF& rect, QGraphicsItem* parent = nullptr);
    explicit SceneBoundary(DiagramScene* scene, qreal x, qreal y, qreal w, qreal h, QGraphicsItem* parent = nullptr);

    ~SceneBoundary();

    SizeGrip *sizeGrip() const;

    ConfirmDialog *confirmDialog() const;

    int type() const override;

public slots:
    void onChangeRequested();

private slots:
    void showComfirmDialogOnMousePosition();

private:
    void init();

private:
    DiagramScene* scene_ = nullptr;

    SizeGripSceneBoundary* sizeGrip_ = nullptr;

    ConfirmDialog* confirmDialog_ = nullptr;

    QTimer* confirmDialogShowTimer_ = nullptr;

    QRectF oldRect_;
};

#endif // SCENEBOUNDARY_H
