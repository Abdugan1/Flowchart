#ifndef NODE_H
#define NODE_H

#include <QtCore/qglobal.h>
#include <QPointF>
#include <QVector>

class Node
{
public:
    bool visited = false;
    qreal globalGoal;
    qreal localGoal;
    QPoint pos;
    QVector<Node*> neighbours;
    Node* parent;
};

#endif // NODE_H
