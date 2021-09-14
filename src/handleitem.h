#ifndef HANDLEITEM_H
#define HANDLEITEM_H

#include <QGraphicsItem>

#include "itemtypes.h"

class HandleManager;

enum PositionFlags
{
    Top         = 0x1,
    Bottom      = 0x2,
    Left        = 0x4,
    TopLeft     = Top | Left,
    BottomLeft  = Bottom | Left,
    Right       = 0x8,
    TopRight    = Top | Right,
    BottomRight = Bottom | Right
};

class HandleItem : public QGraphicsItem
{
public:
    enum {Type = ItemTypes::HandleItemType};

public:
    explicit HandleItem(PositionFlags positionFlags, QGraphicsItem* parent = nullptr);

    PositionFlags positionFlags() const;
    void setPositionFlags(PositionFlags newPositionFlags);

    HandleManager *handleManager() const;
    virtual void setHandleManager(HandleManager *newHandleManager);

    int type() const override;

private:
    PositionFlags positionFlags_;
    HandleManager* handleManager_ = nullptr;
};

#endif // HANDLEITEM_H
