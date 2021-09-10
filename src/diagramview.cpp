#include "diagramview.h"
#include "diagramitem.h"
#include "infolabel.h"
#include "diagramscene.h"
#include "sceneboundary.h"
#include "graphicsitemgroup.h"

#include "constants.h"
#include "internal.h"

#include <QtEvents>
#include <QDebug>
#include <QGraphicsSceneHoverEvent>
#include <QMimeData>
#include <QFileInfo>
#include <QMessageBox>
#include <QMenu>
#include <QRubberBand>
#include <QElapsedTimer>
#include <QGridLayout>

DiagramView::DiagramView(DiagramScene *scene, QWidget *parent)
    : QGraphicsView(scene, parent)
    , scene_(scene)
{
    init();
}

void DiagramView::updateDiagramItemCountInfoText()
{
    if (scene_->diagramItems().count() != lastDiagramCount_) {
        lastDiagramCount_ = scene_->diagramItems().count();
        diagramItemCountLabel_->setText(Constants::DiagramView::DiagramCountInfoText.arg(lastDiagramCount_));

    }
}

void DiagramView::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() == Qt::CTRL) {
        setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        QPoint angle = event->angleDelta(); // Wheel
        QPoint pixel = event->pixelDelta(); // Touchpad
        bool zoomIn = true;
        if (!angle.isNull() && angle.y() < 0) {
            zoomIn = false;
        } else if (!pixel.isNull() && pixel.y() < 0) {
            zoomIn = false;
        }
        zoomIn ? scale(1.1, 1.1) : scale(1 / 1.1, 1 / 1.1);
        qDebug() << transform();
    } else {
        QGraphicsView::wheelEvent(event);
    }
}

void DiagramView::mousePressEvent(QMouseEvent *event)
{
    clickedPos_ = event->pos();
    if (event->modifiers() == Qt::CTRL) {
            setInteractive(false);
            setDragMode(QGraphicsView::ScrollHandDrag);

    } else if (event->button() == Qt::RightButton) {
        contextMenu_->exec(mapToGlobal(event->pos()));
    } else  {
        QList<QGraphicsItem*> items = scene()->items(mapToScene(event->pos()));
        if (items.empty()/* || (qgraphicsitem_cast<SceneBoundary*>(items.first()))*/) {
            if (event->button() == Qt::LeftButton) {
                initRubberBand();
            }
        }
    }

    QGraphicsView::mousePressEvent(event);
}

void DiagramView::mouseMoveEvent(QMouseEvent *event)
{
    movedPos_ = event->pos();

    QPoint currMousePos = mapToScene(movedPos_.toPoint()).toPoint();
    mousePosLabel_->setText(Constants::DiagramView::CurrentMousePosInfoText.arg(currMousePos.x()).arg(currMousePos.y()));

    if (rubberBandActive_)
        updateRubberBand();

    if (event->button() == Qt::LeftButton && event->modifiers() != Qt::CTRL
            && !isInteractive()) {
        setInteractive(true);
        setDragMode(NoDrag);
    }
    QGraphicsView::mouseMoveEvent(event);
}

void DiagramView::mouseReleaseEvent(QMouseEvent *event)
{
    if (rubberBandActive_)
        finishRubberBand();

    setInteractive(true);
    setDragMode(NoDrag);
    QGraphicsView::mouseReleaseEvent(event);
}

void DiagramView::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        QFileInfo fileInfo(event->mimeData()->urls().first().toLocalFile());
        if (fileInfo.suffix() == "json")
            event->acceptProposedAction();
    }
}

void DiagramView::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        QFileInfo fileInfo(event->mimeData()->urls().first().toLocalFile());
        if (fileInfo.suffix() == "json")
            event->acceptProposedAction();
    }
}

void DiagramView::dropEvent(QDropEvent *event)
{
    QMessageBox::StandardButton reply =
            QMessageBox::warning(this, tr("Warning"),
                                 tr("Are you sure you want to open another flowchart?"
                                    "\nThe current flowchart will be lost."),
                                 QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        QString fileName = event->mimeData()->urls().first().toLocalFile();
        emit saveFileDropped(fileName);
    }
}

void DiagramView::drawBackground(QPainter *painter, const QRectF &rect)
{
    QGraphicsView::drawBackground(painter, rect);

    int left = int(rect.left() - (int(rect.left()) % Constants::DiagramScene::GridSize));
    int top  = int(rect.top()  - (int(rect.top())  % Constants::DiagramScene::GridSize));

    QVector<QPointF> points;
    points.reserve(rect.right() * rect.bottom());
    for (int x = left; x < rect.right(); x += Constants::DiagramScene::GridSize) {
        for (int y = top; y < rect.bottom(); y += Constants::DiagramScene::GridSize) {
            points.append(QPointF(x, y));
        }
    }

    painter->drawPoints(points.data(), points.size());
}

void DiagramView::init()
{
    initContextMenu();

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setObjectName("graphicsView");
    setRenderHint(QPainter::Antialiasing);

    setMouseTracking(true);
    setAcceptDrops(true);

    setFont(QFont(":/fonts/Montserrat-Regular.ttf", 12));

    diagramItemCountLabel_ = new InfoLabel(Constants::DiagramView::DiagramCountInfoText);
    diagramItemCountLabel_->setFont(font());
    diagramItemCountLabel_->setAlignment(Qt::AlignRight);
    diagramItemCountLabel_->setText(Constants::DiagramView::DiagramCountInfoText.arg("0"));

    mousePosLabel_ = new InfoLabel(Constants::DiagramView::CurrentMousePosInfoText.arg("-1").arg("-1"));
    mousePosLabel_->setFont(font());
    mousePosLabel_->setAlignment(Qt::AlignRight);

    QVBoxLayout* vLayout = new QVBoxLayout;
    vLayout->addWidget(diagramItemCountLabel_);
    vLayout->addWidget(mousePosLabel_);
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->setSpacing(0);

    QSpacerItem* hSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    QSpacerItem* vSpacer = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

    QGridLayout* gLayout = new QGridLayout;
    gLayout->addItem(hSpacer, 0, 0);
    gLayout->addItem(vSpacer, 0, 1);
    gLayout->addLayout(vLayout, 1, 1);

    gLayout->setContentsMargins(0, 0, 0, 0);

    setLayout(gLayout);
}

void DiagramView::initContextMenu()
{
    contextMenu_ = new QMenu(this);

    QAction* copyAction  = new QAction(tr("&Copy"));
    connect(copyAction, &QAction::triggered, this,
            [this]()
    {
        QPointF pos = mapToScene(mapFromGlobal(contextMenu_->pos()));
        QList<QGraphicsItem*> itemsUnderMouse = scene()->items(pos);
        emit copyActionTriggered(itemsUnderMouse);
    });

    QAction* pasteAction = new QAction(tr("&Paste"));
    connect(pasteAction, &QAction::triggered, this,
            [this]()
    {
        emit pasteActionTriggered(mapToScene(mapFromGlobal(contextMenu_->pos())));
    });

    QAction* deleteAction = new QAction(tr("&Delete"));
    connect(deleteAction, &QAction::triggered, this,
            [this]()
    {
        QPointF pos = mapToScene(mapFromGlobal(contextMenu_->pos()));
        QList<QGraphicsItem*> itemsUnderMouse = scene()->items(pos);
        emit deleteActionTriggered(itemsUnderMouse);
    });

    contextMenu_->addAction(copyAction);
    contextMenu_->addAction(pasteAction);
    contextMenu_->addAction(deleteAction);
}

void DiagramView::initRubberBand()
{
    if (!rubberBand_)
        rubberBand_ = new QRubberBand(QRubberBand::Rectangle, this);
    rubberBand_->setGeometry(QRect(clickedPos_.toPoint(), QSize()));
    rubberBand_->show();
    rubberBandActive_ = true;
}

void DiagramView::updateRubberBand()
{
    rubberBand_->setGeometry(QRect(clickedPos_.toPoint(),
                                   movedPos_.toPoint()).normalized());
}

void DiagramView::finishRubberBand()
{
    rubberBand_->hide();
    rubberBandActive_ = false;
    emit rubberBandSelectingFinished(QRectF(mapToScene(rubberBand_->geometry().topLeft()),
                                            mapToScene(rubberBand_->geometry().bottomRight())));
}
