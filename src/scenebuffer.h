#ifndef SCENEBUFFER_H
#define SCENEBUFFER_H

#include <QtCore/qglobal.h>
#include <QPainterPath>

class ItemProperties {
public:
    QPainterPath path() const;
    void setPath(QPainterPath newPath);

    const QString &text() const;
    void setText(const QString &newText);

    QPointF pos() const;
    void setPos(QPointF newPos);

    int diagramType() const;
    void setDiagramType(int newDiagramType);

private:
    QPainterPath path_;
    QString text_;
    QPointF pos_;
    int diagramType_ = -1;
};

class SceneBuffer
{
public:
    const QList<ItemProperties> &copiedItemsProperties() const;
    void setCopiedItemsProperties(const QList<ItemProperties> &newCopiedItemsProperties);

    bool groupCopied() const;
    void setGroupCopied(bool newGroupCopied);

    void reset();

private:
    QList<ItemProperties> copiedItemsProperties_;
    bool groupCopied_ = false;
};

#endif // SCENEBUFFER_H
