#ifndef DIAGRAMSCENE_H
#define DIAGRAMSCENE_H

#include <QGraphicsScene>

class DiagramScene : public QGraphicsScene
{
public:
    enum DefaultSize {
        Width = 645,
        Height = 975
    };
public:
    DiagramScene(QObject* parent = nullptr);
};

#endif // DIAGRAMSCENE_H
