#include "confirmdialog.h"

#include <QToolButton>
#include <QBoxLayout>

ConfirmDialog::ConfirmDialog(QWidget *parent)
    : QDialog(parent)
{
    const int s = 15;

    confirmButton_ = new QToolButton;
    confirmButton_->setIcon(QPixmap(":/images/check_mark.jpg"));
    confirmButton_->setIconSize(QSize(s, s));

    cancelButton_ = new QToolButton;
    cancelButton_->setIcon(QPixmap(":/images/cancel_mark.jpg"));
    cancelButton_->setIconSize(QSize(s, s));

    connect(confirmButton_, &QToolButton::clicked, this, [this]() {
        hide();
        emit confirmClicked();
    });

    connect(cancelButton_,  &QToolButton::clicked, this, [this]() {
        hide();
        emit cancelClicked();
    });

    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(confirmButton_);
    layout->addWidget(cancelButton_);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);


    setLayout(layout);
    setWindowFlags(Qt::FramelessWindowHint
                   | Qt::WindowStaysOnTopHint);
}
