#include "sceneboundary.h"
#include "diagramscene.h"
#include "sizegripsceneboundary.h"
#include "confirmdialog.h"

#include "constants.h"

#include <QTimer>

SceneBoundary::SceneBoundary(DiagramScene *scene, const QRectF &rect, QGraphicsItem *parent)
    : QGraphicsRectItem(rect, parent)
    , scene_(scene)
    , sizeGrip_(new SizeGripSceneBoundary(this))
    , confirmDialog_(new ConfirmDialog)
    , confirmDialogShowTimer_(new QTimer(this))
    , oldRect_(rect)
{
    init();
}

SceneBoundary::SceneBoundary(DiagramScene *scene, qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent)
    : QGraphicsRectItem(x, y, w, h, parent)
    , scene_(scene)
    , sizeGrip_(new SizeGripSceneBoundary(this))
    , confirmDialog_(new ConfirmDialog)
    , confirmDialogShowTimer_(new QTimer(this))
    , oldRect_(x, y, w, h)
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
    setBrush(Qt::green); // Temporary for debugging

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

    connect(sizeGrip_, &SizeGripSceneBoundary::resizeBeenMade, this, [this]() {
        if (this->isVisible()) {
            confirmDialog_->hide();
            confirmDialogShowTimer_->start(Constants::SceneBoundary::ConfirmShowDelay);
        }
    });
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

void SceneBoundary::onChangeRequested()
{
    this->show();
}

void SceneBoundary::showComfirmDialogOnMousePosition()
{
    confirmDialog_->move(QCursor::pos());
    confirmDialog_->show();
}
