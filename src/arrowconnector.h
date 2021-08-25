#ifndef ARROWCONNECTOR_H
#define ARROWCONNECTOR_H

#include <QtCore/QObject>
#include <QtCore/qglobal.h>

#include "node.h"
#include <QLineF>
#include <QVarLengthArray>

#include "constants.h"

class ArrowItem;

class ArrowConnector : public QObject
{
    Q_OBJECT

public:
    explicit ArrowConnector(QObject *parent = nullptr);

    QList<QLineF> getConnectionPath(const QPoint& startPoint, const QPoint& endPoint);

public slots:
    void updateArrow(ArrowItem* arrow, const QPoint& startPoint, const QPoint& endPoint);

private:
    void initNodes();

    Node* getNodeFromPos(const QPoint& point);

private:
    QVarLengthArray<Node, (Constants::DiagramScene::A4Width + 1) * (Constants::DiagramScene::A4Height + 1)> nodes_;
};

#endif // ARROWCONNECTOR_H
