#ifndef SCENEBUFFER_H
#define SCENEBUFFER_H

#include <QtCore/qglobal.h>
#include <QPainterPath>

class ItemProperties {
public:
    QPainterPath path;
    QString text;
    QPointF pos;
    int diagramType;
};

class SceneBuffer
{
public:
    QList<ItemProperties> itemProperties;
    bool isGroup = false;

    void reset()
    {
        itemProperties.clear();
        isGroup = false;
    }
};

#endif // SCENEBUFFER_H
