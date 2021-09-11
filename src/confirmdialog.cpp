#include "confirmdialog.h"

#include <QToolButton>
#include <QBoxLayout>

ConfirmDialog::ConfirmDialog(QWidget *parent)
    : QDialog(parent)
{
    confirmButton_ = new QToolButton;
    confirmButton_->setObjectName("confirmDialogButton");
    confirmButton_->setIcon(QPixmap(":/images/check_mark.png"));
    confirmButton_->setToolTip(tr("Confirm"));

    cancelButton_ = new QToolButton;
    cancelButton_->setObjectName("confirmDialogButton");
    cancelButton_->setIcon(QPixmap(":/images/cancel_mark.png"));
    cancelButton_->setToolTip(tr("Cancel"));

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
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    setAttribute(Qt::WA_AlwaysShowToolTips);
}
