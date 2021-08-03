#ifndef DIAGRAMVIEW_H
#define DIAGRAMVIEW_H

#include <QGraphicsView>

class DiagramView : public QGraphicsView
{
    Q_OBJECT

public:
    DiagramView(QWidget* parent = nullptr);
    DiagramView(QGraphicsScene* scene, QWidget* parent = nullptr);

signals:
    void rubberBandSelectingFinished();
    void saveFileDropped(const QString& fileName);

protected:
    void wheelEvent(QWheelEvent *event)         override;
    void mousePressEvent(QMouseEvent *event)    override;
    void mouseMoveEvent(QMouseEvent *event)     override;
    void mouseReleaseEvent(QMouseEvent *event)  override;

    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent  *event)  override;
    void dropEvent(QDropEvent *event) override;

private:
    void init();
    bool isRubberBandFinishedSelecting(const QRect& rubberBandRect,
                                       const QPointF& fromScenePoint,
                                       const QPointF& toScenePoint);
};

#endif // DIAGRAMVIEW_H
