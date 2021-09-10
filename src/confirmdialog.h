#ifndef CONFIRMDIALOG_H
#define CONFIRMDIALOG_H

#include <QDialog>

class QToolButton;

class ConfirmDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfirmDialog(QWidget *parent = nullptr);

signals:
    void confirmClicked();
    void cancelClicked();

private:
    QToolButton* confirmButton_ = nullptr;
    QToolButton* cancelButton_  = nullptr;
};

#endif // CONFIRMDIALOG_H
