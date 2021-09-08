#ifndef SIZEGRIPSCENEBOUNDARY_H
#define SIZEGRIPSCENEBOUNDARY_H

#include <QtCore/qglobal.h>

#include "sizegrip.h"

class SceneBoundary;

class SizeGripSceneBoundary : public SizeGrip
{
    Q_OBJECT

public:
    explicit SizeGripSceneBoundary(SceneBoundary* sceneBoundary, QObject* parent = nullptr);

protected:
    void resizeLogic() override;

private:
    SceneBoundary* sceneBoundary_ = nullptr;
};

#endif // SIZEGRIPSCENEBOUNDARY_H
