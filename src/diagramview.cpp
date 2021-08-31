#include "diagramview.h"
#include "diagramitem.h"
#include "arrowitem.h"

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

DiagramView::DiagramView(QWidget *parent)
    : QGraphicsView(parent)
{
    init();
}

DiagramView::DiagramView(QGraphicsScene *scene, QWidget *parent)
    : QGraphicsView(scene, parent)
{
    init();
}

void DiagramView::updateDiagramCountInfoTextArea()
{
    if (internal::getDiagramItemsFromQGraphics(
                items()).count() == lastDiagramCount_) {
        update();

    } else {
        QRectF viewportRect = viewport()->rect();
        const QString text = Constants::DiagramView::DiagramCountInfoText.arg("XXXXXX");
        QFontMetrics fm(font());
        update(viewportRect.width()  - fm.horizontalAdvance(text),
               viewportRect.height() - fm.height() * 2,
               fm.horizontalAdvance(text), fm.height());
    }
}

void DiagramView::updateCurrentMousePosInfoTextArea()
{
    QRectF viewportRect = viewport()->rect();
    const QString text = Constants::DiagramView::CurrentMousePosInfoText.arg("XXXX","XXXX");
    QFontMetrics fm(font());
    update(viewportRect.width()  - fm.horizontalAdvance(text),
           viewportRect.height() - fm.height(),
           fm.horizontalAdvance(text), fm.height());
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
        if (items.empty()) {
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

    updateCurrentMousePosInfoTextArea();

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

void DiagramView::paintEvent(QPaintEvent *event)
{
    QGraphicsView::paintEvent(event);
    QRectF viewportRect = viewport()->rect();
    QFontMetrics fm(font());

    // Setup current mouse position info text
    QPoint currMousePos = mapToScene(mapFromGlobal(QCursor::pos())).toPoint();
    QString currMousePosInfoText =
            Constants::DiagramView::CurrentMousePosInfoText.arg(currMousePos.x()).arg(currMousePos.y());
    QPointF pointMouse(viewportRect.width()  - fm.horizontalAdvance(currMousePosInfoText),
                       viewportRect.height() - fm.descent());

    // Setup current diagram items count info text
    lastDiagramCount_ = internal::getDiagramItemsFromQGraphics(items()).count();
    QString diagramCountInfoText = Constants::DiagramView::DiagramCountInfoText.arg(lastDiagramCount_);
    QPointF pointDiagram(viewportRect.width()  - fm.horizontalAdvance(diagramCountInfoText),
                         viewportRect.height() - fm.height());

    QPainter painter;
    painter.begin(viewport());

    painter.setFont(font());
    painter.drawText(pointDiagram, diagramCountInfoText);
    painter.drawText(pointMouse, currMousePosInfoText);

    painter.end();
}

void DiagramView::drawBackground(QPainter *painter, const QRectF &rect)
{
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

    painter->drawRect(QRectF(0, 0, Constants::DiagramScene::A4Width, Constants::DiagramScene::A4Height));
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
