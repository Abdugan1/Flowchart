#ifndef DIAGRAMVIEW_H
#define DIAGRAMVIEW_H

#include <QGraphicsView>

class QMenu;
class QRubberBand;

class DiagramView : public QGraphicsView
{
    Q_OBJECT

public:
    DiagramView(QWidget* parent = nullptr);
    DiagramView(QGraphicsScene* scene, QWidget* parent = nullptr);

signals:
    void rubberBandSelectingFinished(const QRectF& rubberBandRect);
    void saveFileDropped(const QString& fileName);
    void copyActionTriggered(const QList<QGraphicsItem*>& items);
    void pasteActionTriggered(const QPointF& posToPaste);
    void deleteActionTriggered(const QList<QGraphicsItem*>& items);

public slots:
    void updateDiagramCountInfoTextArea();
    void updateCurrentMousePosInfoTextArea();

protected:
    void wheelEvent(QWheelEvent *event)         override;
    void mousePressEvent(QMouseEvent *event)    override;
    void mouseMoveEvent(QMouseEvent *event)     override;
    void mouseReleaseEvent(QMouseEvent *event)  override;

    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent  *event)  override;
    void dropEvent(QDropEvent *event) override;

    void paintEvent(QPaintEvent *event) override;

private:
    void init();

    void initContextMenu();

    void initRubberBand();
    void updateRubberBand();
    void finishRubberBand();

    bool isRubberBandFinishedSelecting(const QRect& rubberBandRect,
                                       const QPointF& fromScenePoint,
                                       const QPointF& toScenePoint);

private:
    int lastDiagramCount_ = 0;

    QPointF clickedPos_;
    QPointF movedPos_;
    bool rubberBandActive_ = false;

    QRubberBand* rubberBand_ = nullptr;
    QMenu* contextMenu_;
};

#endif // DIAGRAMVIEW_H
