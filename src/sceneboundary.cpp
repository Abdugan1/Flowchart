#include "sceneboundary.h"
#include "diagramscene.h"
#include "sizegripsceneboundary.h"

#include "constants.h"

SceneBoundary::SceneBoundary(DiagramScene *scene, const QRectF &rect, QGraphicsItem *parent)
    : QGraphicsRectItem(rect, parent)
    , scene_(scene)
    , sizeGrip_(new SizeGripSceneBoundary(this))
{
    init();
}

SceneBoundary::SceneBoundary(DiagramScene *scene, qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent)
    : QGraphicsRectItem(x, y, w, h, parent)
    , scene_(scene)
    , sizeGrip_(new SizeGripSceneBoundary(this))
{
    init();
}

void SceneBoundary::init()
{
    QRectF rect(-Constants::DiagramScene::InitSize,
                -Constants::DiagramScene::InitSize,
                 Constants::DiagramScene::InitSize * 2,
                 Constants::DiagramScene::InitSize * 2);
    sizeGrip_->setMaxGripArea(rect);
}

SizeGrip *SceneBoundary::sizeGrip() const
{
    return sizeGrip_;
}
