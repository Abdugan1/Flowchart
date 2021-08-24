#include "arrowconnector.h"
#include "arrowitem.h"

#include "constants.h"

#include <QDebug>

ArrowConnector::ArrowConnector(QObject *parent)
    : QObject(parent)
{
    initNodes();
}

QList<QLineF> ArrowConnector::getConnectionPath(const QPoint &startPoint, const QPoint &endPoint)
{
    const int w = Constants::DiagramScene::A4Width  / Constants::DiagramScene::GridSize + 1;
    const int h = Constants::DiagramScene::A4Height / Constants::DiagramScene::GridSize + 1;

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            nodes_[y * w + x].visited = false;
            nodes_[y * w + x].globalGoal = INFINITY;
            nodes_[y * w + x].localGoal  = INFINITY;
            nodes_[y * w + x].parent = nullptr;
        }
    }

    auto distance = [](Node* a, Node* b)
    {
        return sqrtf((a->pos.x() - b->pos.x()) * (a->pos.x() - b->pos.x()) +
                     (a->pos.y() - b->pos.y()) * (a->pos.y() - b->pos.y()));
    };

    auto heuristic = [distance](Node* a, Node* b)
    {
        return distance(a, b);
    };

    Node* nodeStart_ = getNodeFromPos(startPoint);
    Node* nodeEnd_   = getNodeFromPos(endPoint);

    Node* nodeCurrent = nodeStart_;
    nodeStart_->localGoal  = 0.0;
    nodeStart_->globalGoal = heuristic(nodeStart_, nodeEnd_);

    QList<Node*> notTestedNodes;
    notTestedNodes.append(nodeStart_);

    while (!notTestedNodes.isEmpty()) {

        while (!notTestedNodes.isEmpty() && notTestedNodes.front()->visited)
            notTestedNodes.pop_front();

        if (notTestedNodes.isEmpty())
            break;

        nodeCurrent = notTestedNodes.front();
        nodeCurrent->visited = true;

        for (auto nodeNeighbour : qAsConst(nodeCurrent->neighbours)) {
            if (!nodeNeighbour->visited)
                notTestedNodes.append(nodeNeighbour);

            qreal possiblyLowerGoal = nodeCurrent->localGoal + distance(nodeCurrent, nodeNeighbour);

            if (possiblyLowerGoal < nodeNeighbour->localGoal) {
                nodeNeighbour->parent = nodeCurrent;
                nodeNeighbour->localGoal = possiblyLowerGoal;
                nodeNeighbour->globalGoal = nodeNeighbour->localGoal + heuristic(nodeNeighbour, nodeEnd_);
            }
        }
    }
    //
    QList<QLineF> lines;
    if (nodeEnd_ != nullptr) {
        Node* p = nodeEnd_;
        while (p->parent != nullptr) {
            QLineF line(p->pos.x(), p->pos.y(), p->parent->pos.x(), p->parent->pos.y());
            lines.append(line);
            p = p->parent;
        }
    }
    return lines;
}

void ArrowConnector::updateArrow(ArrowItem *arrow, const QPoint &startPoint, const QPoint &endPoint)
{
    qDebug() << "updating!";
    QList<QLineF> lines = getConnectionPath(startPoint, endPoint);
    arrow->setPathShape(lines);
}

void ArrowConnector::initNodes()
{
    const int w = Constants::DiagramScene::A4Width  / Constants::DiagramScene::GridSize + 1;
    const int h = Constants::DiagramScene::A4Height / Constants::DiagramScene::GridSize + 1;
    nodes_.resize(w * h);
    qreal x = 0.0;
    qreal y = 0.0;
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            nodes_[i * w + j].pos.setX(x);
            nodes_[i * w + j].pos.setY(y);
            nodes_[i * w + j].parent = nullptr;
            nodes_[i * w + j].visited = false;

            x += Constants::DiagramScene::GridSize;
        }
        x = 0.0;
        y += Constants::DiagramScene::GridSize;
    }

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            if (y > 0)
                nodes_[y * w + x].neighbours.push_back(&nodes_[(y - 1) * w + (x + 0)]);
            if (y < h - 1)
                nodes_[y * w + x].neighbours.push_back(&nodes_[(y + 1) * w + (x + 0)]);
            if (x > 0)
                nodes_[y * w + x].neighbours.push_back(&nodes_[(y + 0) * w + (x - 1)]);
            if (x < w - 1)
                nodes_[y * w + x].neighbours.push_back(&nodes_[(y + 0) * w + (x + 1)]);
        }
    }
}

Node *ArrowConnector::getNodeFromPos(const QPoint &point)
{
    const int w = Constants::DiagramScene::A4Width  / Constants::DiagramScene::GridSize + 1;
    QPoint nodePos(point.x() / Constants::DiagramScene::GridSize,
                   point.y() / Constants::DiagramScene::GridSize);
    return (&nodes_[nodePos.y() * w + nodePos.x()]);
}
