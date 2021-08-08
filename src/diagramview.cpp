#include "diagramview.h"
#include "diagramitem.h"

#include "internal.h"

#include <QtEvents>
#include <QDebug>
#include <QGraphicsSceneHoverEvent>
#include <QMimeData>
#include <QFileInfo>
#include <QMessageBox>
#include <QMenu>

const QString DiagramCountInfoText      = QObject::tr("Diagram count: %1");
const QString CurrentMousePosInfoText   = QObject::tr("Current position (%1, %2)");

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
        const QString text = DiagramCountInfoText.arg("XXXXXX");
        QFontMetrics fm(font());
        update(viewportRect.width()  - fm.horizontalAdvance(text),
               viewportRect.height() - fm.height() * 2,
               fm.horizontalAdvance(text), fm.height());
    }
}

void DiagramView::updateCurrentMousePosInfoTextArea()
{
    QRectF viewportRect = viewport()->rect();
    const QString text = CurrentMousePosInfoText.arg("XXXX","XXXX");
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
    if (event->modifiers() == Qt::CTRL) {
        setInteractive(false);
        setDragMode(QGraphicsView::ScrollHandDrag);
    }
    QGraphicsView::mousePressEvent(event);
}

void DiagramView::mouseMoveEvent(QMouseEvent *event)
{
    updateCurrentMousePosInfoTextArea();

    QList<QGraphicsItem*> itemsUnderMouse = items(event->pos());

    for (auto item : qAsConst(itemsUnderMouse)) {
        QGraphicsSceneHoverEvent hoverMoveEvent(QEvent::GraphicsSceneHoverMove);
        scene()->sendEvent(item, &hoverMoveEvent);
    }

    if (event->buttons() & Qt::LeftButton && event->modifiers() != Qt::CTRL
            && !isInteractive()) {
        setInteractive(true);
        setDragMode(NoDrag);
        setDragMode(RubberBandDrag);
    }
    QGraphicsView::mouseMoveEvent(event);
}

void DiagramView::mouseReleaseEvent(QMouseEvent *event)
{
    setInteractive(true);
    setDragMode(NoDrag);
    setDragMode(RubberBandDrag);
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
            CurrentMousePosInfoText.arg(currMousePos.x()).arg(currMousePos.y());
    QPointF pointMouse(viewportRect.width()  - fm.horizontalAdvance(currMousePosInfoText),
                       viewportRect.height() - fm.descent());

    // Setup current diagram items count info text
    lastDiagramCount_ = internal::getDiagramItemsFromQGraphics(items()).count();
    QString diagramCountInfoText = DiagramCountInfoText.arg(lastDiagramCount_);
    QPointF pointDiagram(viewportRect.width()  - fm.horizontalAdvance(diagramCountInfoText),
                         viewportRect.height() - fm.height());

    QPainter painter;
    painter.begin(viewport());

    painter.setFont(font());
    painter.drawText(pointDiagram, diagramCountInfoText);
    painter.drawText(pointMouse, currMousePosInfoText);

    painter.end();
}

void DiagramView::contextMenuEvent(QContextMenuEvent *event)
{
    contextMenu_->exec(event->globalPos());
}

void DiagramView::init()
{
    initContextMenu();

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setDragMode(QGraphicsView::RubberBandDrag);
    setMouseTracking(true);
    setAcceptDrops(true);
    setFont(QFont(":/fonts/Montserrat-Regular.ttf", 12));

    connect(this, &DiagramView::rubberBandChanged, this,
            [this](QRect rubberBandRect, QPointF fromScenePoint, QPointF toScenePoint)
    {
        if (isRubberBandFinishedSelecting(rubberBandRect, fromScenePoint, toScenePoint))
            emit rubberBandSelectingFinished();
    });
}

void DiagramView::initContextMenu()
{
    contextMenu_ = new QMenu(this);

    QAction* copyAction  = new QAction(tr("&Copy"));
    connect(copyAction, &QAction::triggered, this,
            [this]()
    {
        QPointF pos = mapToScene(mapFromGlobal(contextMenu_->pos()));
        QList<QGraphicsItem*> itemsUnderMouse = scene()->items(pos, Qt::IntersectsItemShape, Qt::AscendingOrder);
        emit copyActionTriggered(itemsUnderMouse);
    });

    QAction* pasteAction = new QAction(tr("&Paste"));
    connect(pasteAction, &QAction::triggered, this,
            [this]()
    {
        emit pasteActionTriggered(mapToScene(mapFromGlobal(contextMenu_->pos())));
    });

    contextMenu_->addAction(copyAction);
    contextMenu_->addAction(pasteAction);
}

bool DiagramView::isRubberBandFinishedSelecting(const QRect &rubberBandRect,
                                                const QPointF &fromScenePoint,
                                                const QPointF &toScenePoint)
{
    return (rubberBandRect.isNull() && fromScenePoint.isNull() && toScenePoint.isNull());
}
