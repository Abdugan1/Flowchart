#include "sceneboundary.h"
#include "diagramscene.h"
#include "sizegripsceneboundary.h"
#include "confirmdialog.h"

#include "constants.h"

#include <QTimer>
#include <QPainter>
#include <QDebug>

SceneBoundary::SceneBoundary(DiagramScene *scene, const QRectF &rect, QGraphicsItem *parent)
    : QGraphicsItem(parent)
    , boundingRect_(rect)
    , rect_(rect)
    , oldRect_(rect)
    , scene_(scene)
    , sizeGrip_(new SizeGripSceneBoundary(this))
    , confirmDialog_(new ConfirmDialog)
    , confirmDialogShowTimer_(new QTimer(this))
{
    init();
}

SceneBoundary::SceneBoundary(DiagramScene *scene, qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent)
    : QGraphicsItem(parent)
    , boundingRect_(x, y, w, h)
    , rect_(x, y, w, h)
    , oldRect_(x, y, w, h)
    , scene_(scene)
    , sizeGrip_(new SizeGripSceneBoundary(this))
    , confirmDialog_(new ConfirmDialog)
    , confirmDialogShowTimer_(new QTimer(this))
{
    init();
}

SceneBoundary::~SceneBoundary()
{
    delete confirmDialog_;
}

void SceneBoundary::init()
{
    QRectF rect(-Constants::DiagramScene::InitSize,
                -Constants::DiagramScene::InitSize,
                 Constants::DiagramScene::InitSize * 2,
                 Constants::DiagramScene::InitSize * 2);
    sizeGrip_->setMaxGripArea(rect);

    calculateAllGraphicsData();

    connect(confirmDialog_, &ConfirmDialog::confirmClicked, this, [this]() {
        this->hide();
        oldRect_ = this->rect();
    });

    connect(confirmDialog_, &ConfirmDialog::cancelClicked, this, [this]() {
        this->hide();
        this->sizeGrip_->resize(oldRect_);
    });

    confirmDialogShowTimer_->setSingleShot(true);
    connect(confirmDialogShowTimer_, &QTimer::timeout, this, &SceneBoundary::showComfirmDialogOnMousePosition);

    connect(sizeGrip_, &SizeGripSceneBoundary::getReadyToResize, this, [this]() {
        this->prepareGeometryChange();
    });

    connect(sizeGrip_, &SizeGripSceneBoundary::resizeBeenMade, this, [this]() {
        calculateAllGraphicsData();
        if (this->isVisible()) {
            confirmDialog_->hide();
            confirmDialogShowTimer_->start(Constants::SceneBoundary::ConfirmDialogShowDelay);
        }
    });
}

void SceneBoundary::calculateAllGraphicsData()
{
    lines_[0].setPoints(rect_.topLeft(), rect_.topRight());
    lines_[1].setPoints(rect_.topRight(), rect_.bottomRight());
    lines_[2].setPoints(rect_.bottomLeft(), rect_.bottomRight());
    lines_[3].setPoints(rect_.topLeft(), rect_.bottomLeft());

    const int d = 5;
    auto getRect = [=](const QLineF& line) {
        return QRectF(line.x1() - d, line.y1() - d,
                      line.x2() - line.x1() + d * 2,
                      line.y2() - line.y1() + d * 2);
    };

    shape_.clear();
    shape_.setFillRule(Qt::WindingFill);

    shape_.addRect(getRect(lines_[0]));
    shape_.addRect(getRect(lines_[1]));
    shape_.addRect(getRect(lines_[2]));
    shape_.addRect(getRect(lines_[3]));

    boundingRect_ = QRectF(rect_.x() - d,
                           rect_.y() - d,
                           rect_.width()  + d * 2,
                           rect_.height() + d * 2);
}

const QRectF &SceneBoundary::rect() const
{
    return rect_;
}

void SceneBoundary::setRect(const QRectF &newRect)
{
    rect_ = newRect;
}

QRectF SceneBoundary::boundingRect() const
{
    return boundingRect_;
}

QPainterPath SceneBoundary::shape() const
{
    return shape_;
}

void SceneBoundary::paint(QPainter *painter,
                          const QStyleOptionGraphicsItem *option,
                          QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->drawRect(rect_);

    painter->setBrush(Qt::NoBrush);
    painter->setPen(Qt::red);
    painter->drawPath(shape_);
}

ConfirmDialog *SceneBoundary::confirmDialog() const
{
    return confirmDialog_;
}

SizeGrip *SceneBoundary::sizeGrip() const
{
    return sizeGrip_;
}

int SceneBoundary::type() const
{
    return Type;
}

void SceneBoundary::showComfirmDialogOnMousePosition()
{
    confirmDialog_->move(QCursor::pos());
    confirmDialog_->show();
}
