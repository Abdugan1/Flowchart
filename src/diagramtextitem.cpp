#include "diagramtextitem.h"

#include <QTextDocument>
#include <QTextBlockFormat>
#include <QTextCursor>
#include <QDebug>
#include <QGuiApplication>
#include <QCursor>

DiagramTextItem::DiagramTextItem(const QString &text, QGraphicsItem *parent)
    : QGraphicsTextItem(text, parent)
    , alignment_(Qt::AlignLeft)
{
    init();
}

DiagramTextItem::DiagramTextItem(QGraphicsItem *parent)
    : QGraphicsTextItem(parent)
    , alignment_(Qt::AlignLeft)
{
    init();
}

void DiagramTextItem::setAlignment(Qt::Alignment alignment)
{
    alignment_ = alignment;
    QTextBlockFormat format;
    format.setAlignment(alignment);
    QTextCursor cursor = textCursor();
    int position = textCursor().position();
    cursor.select(QTextCursor::Document);
    cursor.mergeBlockFormat(format);
    cursor.clearSelection();
    cursor.setPosition(position);
    setTextCursor(cursor);
}

void DiagramTextItem::updateGeometry(int, int, int)
{
    updateGeometry();
}

void DiagramTextItem::updateGeometry()
{
    QPointF topRightPrev = boundingRect().topRight();
    setTextWidth(-1);
    setTextWidth(boundingRect().width());
    setAlignment(alignment_);
    QPointF topRight = boundingRect().topRight();

    if (alignment_ & Qt::AlignRight)
        setPos(pos() + (topRightPrev - topRight));
}

void DiagramTextItem::updatePosition()
{
    qreal width  = boundingRect().width();
    qreal height = boundingRect().height();
    QPointF parentCenter = parentItem()->boundingRect().center();

    qreal x = parentCenter.x() - width  / 2;
    qreal y = parentCenter.y() - height / 2;

    setPos(x, y);
}

void DiagramTextItem::init()
{
    setTextInteractionFlags(Qt::NoTextInteraction);
    QFont font("Times New Roman", 14);
    font.setStyleStrategy(QFont::PreferAntialias);
    setFont(font);
    setAcceptHoverEvents(true);

    updateGeometry();
    connect(document(), SIGNAL(contentsChange(int,int,int)),
            this,       SLOT(updateGeometry(int,int,int))
            );
    connect(document(), &QTextDocument::contentsChanged,
            this,       &DiagramTextItem::updatePosition);
}

void DiagramTextItem::setTextInteraction(bool on)
{
    if (on && textInteractionFlags() == Qt::NoTextInteraction) {
        setTextInteractionFlags(Qt::TextEditorInteraction);
        setFocus(Qt::MouseFocusReason);
        setSelected(true);
        QTextCursor cursor = textCursor();
        cursor.select(QTextCursor::Document);
        setTextCursor(cursor);

    } else if (!on && textInteractionFlags() == Qt::TextEditorInteraction) {
        setTextInteractionFlags(Qt::NoTextInteraction);
        QTextCursor cursor = textCursor();
        cursor.clearSelection();
        setTextCursor(cursor);
        clearFocus();
    }
}

void DiagramTextItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    if (textInteractionFlags() == Qt::TextEditorInteraction)
        QGuiApplication::setOverrideCursor(QCursor(Qt::IBeamCursor));
    QGraphicsTextItem::hoverEnterEvent(event);
}

void DiagramTextItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    QGuiApplication::restoreOverrideCursor();
    QGraphicsTextItem::hoverLeaveEvent(event);
}

void DiagramTextItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if (!QGuiApplication::overrideCursor()
            && textInteractionFlags() == Qt::TextEditorInteraction)
        QGuiApplication::setOverrideCursor(QCursor(Qt::IBeamCursor));
    QGraphicsTextItem::hoverMoveEvent(event);
}
