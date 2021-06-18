#ifndef DIAGRAMTEXTITEM_H
#define DIAGRAMTEXTITEM_H

#include <QGraphicsTextItem>

class DiagramTextItem : public QGraphicsTextItem
{
    Q_OBJECT
public:
    DiagramTextItem(const QString& text, QGraphicsItem* parent = nullptr);
    DiagramTextItem(QGraphicsItem* parent = nullptr);
    void setAlignment(Qt::Alignment alignment);

public slots:
    void updateGeometry(int, int, int);
    void updateGeometry();
    void updatePosition();

private:
    void init();

private:
    Qt::Alignment alignment_;
};

#endif // DIAGRAMTEXTITEM_H
