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

static const qreal DefaultPenWidth  = 1.0;
static const qreal SelectedPenWidth = 2.0;

}

namespace DiagramScene {

static const int InitSize = 3000;

static const int A4Width  = 680;
static const int A4Height = 975;

static const int GridSize = 20;

}

namespace DiagramView {

const QString DiagramCountInfoText      = QObject::tr("Diagram count: %1");
const QString CurrentMousePosInfoText   = QObject::tr("Current position (%1, %2)");

}

namespace HandleItemAppearArea {

static const qreal Margin = 3.0;

}

namespace SizeHandleItem {

static const qreal VisibleWidth  = 8.0;
static const qreal VisibleHeight = 8.0;

static const qreal OverralWidth  = 16.0;
static const qreal OverralHeight = 16.0;

}

namespace ArrowHandleItem {

static const qreal InnerCircleDiameter = 10.0;
static const qreal OuterStrokeDiameter = 16.0;

static const qreal OverralWidth  = 16.0;
static const qreal OverralHeight = 16.0;

static const qreal OuterStrokePenWidth = 1.0;

}

namespace ArrowManagerItem {

static const qreal Margin = 20.0;

}

namespace GraphicsItemGroup {

static const int Margin = 0;
static const qreal PenWidth = 1.0;

}

namespace SceneBoundary {

static const int ConfirmDialogShowDelay = 200;

}

}

#endif // CONSTANTS_H
