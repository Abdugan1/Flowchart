#ifndef INTERNAL_H
#define INTERNAL_H

class QPointF;

namespace internal {

QPointF getPointByStep(QPointF point, int step);
int map(int value, int fromLow, int fromHigh, int toLow, int toHigh);

}

#endif // INTERNAL_H
