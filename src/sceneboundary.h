#ifndef SCENEBOUNDARY_H
#define SCENEBOUNDARY_H

#include <QGraphicsRectItem>
#include <QtCore/qglobal.h>

class DiagramScene;
class SizeGrip;
class SizeGripSceneBoundary;

class SceneBoundary : public QGraphicsRectItem
{
public:
    explicit SceneBoundary(DiagramScene* scene, const QRectF& rect, QGraphicsItem* parent = nullptr);
    explicit SceneBoundary(DiagramScene* scene, qreal x, qreal y, qreal w, qreal h, QGraphicsItem* parent = nullptr);

    SizeGrip *sizeGrip() const;

private:
    void init();

private:
    DiagramScene* scene_;
    SizeGripSceneBoundary* sizeGrip_;
};

#endif // SCENEBOUNDARY_H
