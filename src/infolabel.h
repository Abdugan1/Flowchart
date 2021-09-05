#ifndef INFOLABEL_H
#define INFOLABEL_H

#include <QLabel>
#include <QtCore/qglobal.h>

class InfoLabel : public QLabel
{
public:
    explicit InfoLabel(QWidget *parent=nullptr, Qt::WindowFlags f=Qt::WindowFlags());
    explicit InfoLabel(const QString &text, QWidget *parent=nullptr, Qt::WindowFlags f=Qt::WindowFlags());
};

#endif // INFOLABEL_H
