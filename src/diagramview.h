#ifndef DIAGRAMVIEW_H
#define DIAGRAMVIEW_H

#include <QGraphicsView>

class InfoLabel;
class DiagramScene;

class QMenu;
class QRubberBand;

class DiagramView : public QGraphicsView
{
    Q_OBJECT

public:
    DiagramView(DiagramScene* scene, QWidget* parent = nullptr);

signals:
    void rubberBandSelectingFinished(const QRectF& rubberBandRect);
    void saveFileDropped(const QString& fileName);
    void copyActionTriggered(const QList<QGraphicsItem*>& items);
    void pasteActionTriggered(const QPointF& posToPaste);
    void deleteActionTriggered(const QList<QGraphicsItem*>& items);

public slots:
    void updateDiagramItemCountInfoText();

protected:
    void wheelEvent(QWheelEvent *event)         override;
    void mousePressEvent(QMouseEvent *event)    override;
    void mouseMoveEvent(QMouseEvent *event)     override;
    void mouseReleaseEvent(QMouseEvent *event)  override;

    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent  *event)  override;
    void dropEvent(QDropEvent *event) override;

    void drawBackground(QPainter *painter, const QRectF &rect) override;

private:
    void init();

    void initContextMenu();

    void initRubberBand();
    void updateRubberBand();
    void finishRubberBand();

private:
    DiagramScene* scene_;

    int lastDiagramCount_ = 0;

    QPointF clickedPos_;
    QPointF movedPos_;
    bool rubberBandActive_ = false;

    QRubberBand* rubberBand_ = nullptr;
    QMenu* contextMenu_;

    InfoLabel* diagramItemCountLabel_ = nullptr;
    InfoLabel* mousePosLabel_ = nullptr;
};

#endif // DIAGRAMVIEW_H
