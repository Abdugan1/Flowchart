#include "diagramscene.h"

DiagramScene::DiagramScene(QObject *parent)
    : QGraphicsScene(parent)
{
    QFont font("Times New Roman", 14);
    setFont(font);
}
