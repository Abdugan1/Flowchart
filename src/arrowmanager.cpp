#include "arrowmanager.h"
#include "arrowhandleitem.h"
#include "handleitemappeararea.h"
#include "diagramitem.h"
#include "arrowitem.h"

#include "constants.h"

#include <QGraphicsScene>
#include <QDebug>

ArrowManager::ArrowManager(DiagramItem *diagramItem, QObject *parent)
    : HandleManager(diagramItem, parent)
    , diagramItem_(diagramItem)
{
    using HIAA = HandleItemAppearArea;

    HandleManager::addHandleItemAppearArea(new HIAA(new ArrowHandleItem(Top   ), this));
    HandleManager::addHandleItemAppearArea(new HIAA(new ArrowHandleItem(Left  ), this));
    HandleManager::addHandleItemAppearArea(new HIAA(new ArrowHandleItem(Right ), this));
    HandleManager::addHandleItemAppearArea(new HIAA(new ArrowHandleItem(Bottom), this));

    updateHandleItemsPositions();
    hideHandleItems();
}

void ArrowManager::emitHandleClicked(ArrowHandleItem *handleItem)
{
    emit handleClicked(handleItem, diagramItem_);
}

void ArrowManager::addArrow(ArrowItem *arrow)
{
    if (arrow->startItem() != arrow->endItem())
        arrows_.append(arrow);
}

void ArrowManager::removeArrow(ArrowItem *arrow)
{
    arrows_.removeAll(arrow);
}

void ArrowManager::removeArrows()
{
    QGraphicsScene* scene = diagramItem_->scene();
    const auto arrowsCopy = arrows_;
    for (auto arrow : arrowsCopy) {
        arrow->startItem()->removeArrow(arrow);
        arrow->endItem()->removeArrow(arrow);
        scene->removeItem(arrow);
        delete arrow;
    }
}

void ArrowManager::updateHandleItemsPositions()
{
    QRectF rect = diagramItem_->pathBoundingRect();

    for (auto appearArea : qAsConst(HandleManager::appearAreaItems())) {
        auto arrowHandle = appearArea->handleItem();

        switch (arrowHandle->positionFlags()) {
        case Top:
            appearArea->setPos(rect.left() + rect.width() / 2,
                         rect.top() - Constants::ArrowManager::Margin);
            break;
        case Left:
            appearArea->setPos(rect.left() - Constants::ArrowManager::Margin,
                         rect.top() + rect.height() / 2);
            break;
        case Right:
            appearArea->setPos(rect.right() + Constants::ArrowManager::Margin,
                         rect.top() + rect.height() / 2);
            break;
        case Bottom:
            appearArea->setPos(rect.left() + rect.width() / 2,
                         rect.bottom() + Constants::ArrowManager::Margin);
            break;

        default:
            break;
        }

    }
}

const QList<ArrowItem *> &ArrowManager::arrows() const
{
    return arrows_;
}

void ArrowManager::updateArrows()
{
    for (auto arrow : qAsConst(arrows_)) {
        if (arrow->startItem() && arrow->endItem())
            arrow->updatePathShape();
    }
}





