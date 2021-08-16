#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QtGlobal>
#include <QObject>

namespace Constants {

namespace DiagramItem {

static const qreal DefaultWidth  = 200.0;
static const qreal DefaultHeight = 80.0;

static const qreal MinWidth  = 80.0;
static const qreal MinHeight = 60.0;

static const int DefaultPenWidth  = 1;
static const int SelectedPenWidth = 2;

}

namespace DiagramScene {

static const int A4Width  = 645;
static const int A4Height = 975;

static const int GridSize = 20;

}

namespace DiagramView {

const QString DiagramCountInfoText      = QObject::tr("Diagram count: %1");
const QString CurrentMousePosInfoText   = QObject::tr("Current position (%1, %2)");

}

namespace HandleItem {

static const qreal VisibleWidth  = 8.0;
static const qreal VisibleHeight = 8.0;

static const qreal OverralHeight = 16.0;
static const qreal OverralWidth  = 16.0;

static const qreal Margin = 3.0;

}

namespace GraphicsItemGroup {

static const int Margin = 16;
static const int PenWidth = 1;

}

}

#endif // CONSTANTS_H
