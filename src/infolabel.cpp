#include "infolabel.h"

InfoLabel::InfoLabel(QWidget *parent, Qt::WindowFlags f)
    : QLabel(parent, f)
{
    setAttribute(Qt::WA_TransparentForMouseEvents, true);
}

InfoLabel::InfoLabel(const QString &text, QWidget *parent, Qt::WindowFlags f)
    : QLabel(text, parent, f)
{
    setAttribute(Qt::WA_TransparentForMouseEvents, true);
}
