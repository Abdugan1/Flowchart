#ifndef DIAGRAMVIEW_H
#define DIAGRAMVIEW_H

#include <QGraphicsView>

class QMenu;

class DiagramView : public QGraphicsView
{
    Q_OBJECT

public:
    DiagramView(QWidget* parent = nullptr);
    DiagramView(QGraphicsScene* scene, QWidget* parent = nullptr);

signals:
    void rubberBandSelectingFinished();
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

    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    void init();
    void initContextMenu();
    bool isRubberBandFinishedSelecting(const QRect& rubberBandRect,
                                       const QPointF& fromScenePoint,
                                       const QPointF& toScenePoint);

private:
    int lastDiagramCount_ = 0;

    QMenu* contextMenu_;
};

#endif // DIAGRAMVIEW_H
