#include "sizegripsceneboundary.h"
#include "sceneboundary.h"

SizeGripSceneBoundary::SizeGripSceneBoundary(SceneBoundary *sceneBoundary, QObject *parent)
    : SizeGrip(sceneBoundary, sceneBoundary->rect(), QSizeF(100, 100), parent)
    , sceneBoundary_(sceneBoundary)
{
}

void SizeGripSceneBoundary::resizeLogic()
{
    sceneBoundary_->setRect(SizeGrip::resizeRect());
}
