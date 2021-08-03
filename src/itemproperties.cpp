#include "itemproperties.h"
#include "diagramitem.h"

#include <QJsonObject>
#include <QJsonArray>

QPointF ItemProperties::pos() const
{
    return pos_;
}

void ItemProperties::setPos(QPointF newPos)
{
    pos_ = newPos;
}

const QSizeF &ItemProperties::size() const
{
    return size_;
}

void ItemProperties::setSize(const QSizeF &newSize)
{
    size_ = newSize;
}

const QString &ItemProperties::text() const
{
    return text_;
}

void ItemProperties::setText(const QString &newText)
{
    text_ = newText;
}

int ItemProperties::diagramType() const
{
    return diagramType_;
}

void ItemProperties::setDiagramType(int newDiagramType)
{
    diagramType_ = newDiagramType;
}

void obtainItemProperties(const DiagramItem *diagramItem, ItemProperties *itemProperties)
{
    itemProperties->setPos (diagramItem->scenePos());
    itemProperties->setSize(diagramItem->path().boundingRect().size());
    itemProperties->setText(diagramItem->text());
    itemProperties->setDiagramType(diagramItem->diagramType());
}

QString stringifiedDiagramType(int diagramType)
{
    QString stringified;
    switch (diagramType) {
    case DiagramItem::Terminal: stringified = "Terminal";   break;
    case DiagramItem::Process:  stringified = "Process";    break;
    case DiagramItem::Desicion: stringified = "Desicion";   break;
    case DiagramItem::InOut:    stringified = "InOut";      break;
    case DiagramItem::ForLoop:  stringified = "ForLoop";    break;
    }
    return stringified;
}

int getDiagramTypeFromString(const QString &strDiagramType)
{
    if (strDiagramType == "Terminal")   return DiagramItem::Terminal;
    if (strDiagramType == "Process")    return DiagramItem::Process;
    if (strDiagramType == "Desicion")   return DiagramItem::Desicion;
    if (strDiagramType == "InOut")      return DiagramItem::InOut;
    if (strDiagramType == "ForLoop")    return DiagramItem::ForLoop;

    return -1;
}

QJsonObject getJsonFromItemsProperties(const QList<ItemProperties> &itemsProperties)
{
    QJsonObject jsonObject;
    QJsonArray  blocks;

    for (auto& itemProperties : qAsConst(itemsProperties)) {
        QJsonObject block;

        block["x"] = itemProperties.pos().x();
        block["y"] = itemProperties.pos().y();

        block["width"]  = itemProperties.size().width();
        block["height"] = itemProperties.size().height();

        block["text"] = itemProperties.text();
        block["type"] = stringifiedDiagramType(itemProperties.diagramType());

        blocks.append(block);
    }

    jsonObject["blocks"] = blocks;

    return jsonObject;
}

QList<ItemProperties> getItemsPropertiesFromJson(const QJsonObject &jsonObject)
{
    QList<ItemProperties> itemsProperties;
    QJsonArray blocks = jsonObject["blocks"].toArray();

    for (const auto& value : qAsConst(blocks)) {
        QJsonObject block = value.toObject();
        ItemProperties itemProperties;

        qreal x = block["x"].toDouble();
        qreal y = block["y"].toDouble();

        qreal width  = block["width"].toDouble();
        qreal height = block["height"].toDouble();

        QString text = block["text"].toString();
        QString type = block["type"].toString();

        itemProperties.setPos(QPointF(x, y));
        itemProperties.setSize(QSizeF(width, height));
        itemProperties.setText(text);
        itemProperties.setDiagramType(getDiagramTypeFromString(type));

        itemsProperties.append(itemProperties);
    }

    return itemsProperties;
}
