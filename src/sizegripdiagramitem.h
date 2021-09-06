#ifndef SIZEGRIPDIAGRAMITEM_H
#define SIZEGRIPDIAGRAMITEM_H

#include "sizegrip.h"

#include <QRectF>

class DiagramItem;
class SizeHandleItem;
class HandleItemAppearArea;

class SizeGripDiagramItem : public SizeGrip
{
    Q_OBJECT

public:
    SizeGripDiagramItem(DiagramItem* diagramItem, QObject* parent = nullptr);

protected:
    void resizeLogic() override;

private:
    DiagramItem* diagramItem_ = nullptr;
};

#endif // SIZEGRIPDIAGRAMITEM_H
