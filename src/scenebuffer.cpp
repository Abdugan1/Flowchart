#include "scenebuffer.h"

QPainterPath ItemProperties::path() const
{
    return path_;
}

void ItemProperties::setPath(QPainterPath newPath)
{
    path_ = newPath;
}

const QString &ItemProperties::text() const
{
    return text_;
}

void ItemProperties::setText(const QString &newText)
{
    text_ = newText;
}

QPointF ItemProperties::pos() const
{
    return pos_;
}

void ItemProperties::setPos(QPointF newPos)
{
    pos_ = newPos;
}

int ItemProperties::diagramType() const
{
    return diagramType_;
}

void ItemProperties::setDiagramType(int newDiagramType)
{
    diagramType_ = newDiagramType;
}


//!----------------------------------------------------------------!//
//!------------------SceneBuffer implementation--------------------!//
//!----------------------------------------------------------------!//


const QList<ItemProperties> &SceneBuffer::copiedItemsProperties() const
{
    return copiedItemsProperties_;
}

void SceneBuffer::setCopiedItemsProperties(const QList<ItemProperties> &newCopiedItemsProperties)
{
    copiedItemsProperties_ = newCopiedItemsProperties;
}

bool SceneBuffer::groupCopied() const
{
    return groupCopied_;
}

void SceneBuffer::setGroupCopied(bool newGroupCopied)
{
    groupCopied_ = newGroupCopied;
}

void SceneBuffer::reset()
{
    copiedItemsProperties_.clear();
    groupCopied_ = false;
}
