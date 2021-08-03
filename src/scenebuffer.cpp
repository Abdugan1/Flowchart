#include "scenebuffer.h"

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

bool SceneBuffer::isEmpty() const
{
    return copiedItemsProperties_.isEmpty();
}

void SceneBuffer::reset()
{
    copiedItemsProperties_.clear();
    groupCopied_ = false;
}
