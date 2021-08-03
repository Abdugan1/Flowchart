#ifndef SCENEBUFFER_H
#define SCENEBUFFER_H

#include <QtCore/qglobal.h>
#include <QPainterPath>

#include "itemproperties.h"

class SceneBuffer
{
public:
    const QList<ItemProperties> &copiedItemsProperties() const;
    void setCopiedItemsProperties(const QList<ItemProperties> &newCopiedItemsProperties);

    bool groupCopied() const;
    void setGroupCopied(bool newGroupCopied);

    bool isEmpty() const;

    void reset();

private:
    QList<ItemProperties> copiedItemsProperties_;
    bool groupCopied_ = false;
};

#endif // SCENEBUFFER_H
