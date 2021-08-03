#ifndef ITEMPROPERTIES_H
#define ITEMPROPERTIES_H

#include <QtCore/qglobal.h>
#include <QPainterPath>

class DiagramItem;

class ItemProperties {
public:
    QPointF pos() const;
    void setPos(QPointF newPos);

    const QSizeF &size() const;
    void setSize(const QSizeF &newSize);

    const QString &text() const;
    void setText(const QString &newText);

    int diagramType() const;
    void setDiagramType(int newDiagramType);

private:
    QPointF pos_;
    QSizeF  size_;
    QString text_;
    int diagramType_ = -1;
};

void obtainItemProperties(const DiagramItem *diagramItem, ItemProperties *itemProperties);

QString stringifiedDiagramType(int diagramType);
int     getDiagramTypeFromString(const QString &strDiagramType);

QJsonObject getJsonFromItemsProperties(const QList<ItemProperties> &itemsProperties);
QList<ItemProperties> getItemsPropertiesFromJson(const QJsonObject &jsonObject);

#endif // ITEMPROPERTIES_H
