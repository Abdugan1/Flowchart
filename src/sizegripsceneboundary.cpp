#include "sizegripsceneboundary.h"
#include "sceneboundary.h"

#include <QtDebug>

SizeGripSceneBoundary::SizeGripSceneBoundary(SceneBoundary *sceneBoundary, QObject *parent)
    : SizeGrip(sceneBoundary, sceneBoundary->rect(), QSizeF(100, 100), parent)
    , sceneBoundary_(sceneBoundary)
{
    qDebug() << SizeGrip::resizeRect();
}

void SizeGripSceneBoundary::resizeLogic()
{
    sceneBoundary_->setRect(SizeGrip::resizeRect());
}
