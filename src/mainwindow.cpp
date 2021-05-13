#include "mainwindow.h"

#include <QGraphicsView>
#include <QGraphicsProxyWidget>
#include <QBoxLayout>
#include <QButtonGroup>
#include <QToolButton>
#include <QLabel>
#include <QToolBox>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    createToolBox();

    scene_ = new DiagramScene(this);
    scene_->setSceneRect(QRectF(0, 0, DiagramScene::Width, DiagramScene::Height));
    scene_->addRect(QRectF(0, 0, DiagramScene::Width, DiagramScene::Height));

    view_ = new QGraphicsView(scene_);
    view_->setRenderHint(QPainter::Antialiasing, true);

    QHBoxLayout * hLayout = new QHBoxLayout;
    hLayout->addWidget(toolBox_);
    hLayout->addWidget(view_);

    QWidget* widget = new QWidget;
    widget->setLayout(hLayout);
    setCentralWidget(widget);
}

void MainWindow::buttonGroupClicked(QAbstractButton* button)
{
    const QList<QAbstractButton*> buttons = buttonGroup_->buttons();
    for (QAbstractButton* myButton : buttons) {
        if (myButton != button)
            button->setChecked(false);
    }

    DiagramItem* diagramItem;

    const DiagramItem::DiagramType type =
            DiagramItem::DiagramType(buttonGroup_->id(button));
    switch (type) {
    case DiagramItem::Terminal:
//        break;
    case DiagramItem::Process:
    case DiagramItem::Desicion:
    case DiagramItem::InOut:
        diagramItem = new DiagramPolygonItem(DiagramPolygonItem::DiagramPolygonType(type));
        break;
    }
    diagramItem->setFlag(QGraphicsItem::ItemIsMovable);

    scene_->addItem(diagramItem);
}

void MainWindow::createToolBox()
{
    buttonGroup_ = new QButtonGroup(this);
    buttonGroup_->setExclusive(false);
    connect(buttonGroup_, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked),
            this, &MainWindow::buttonGroupClicked);
    QGridLayout* layout = new QGridLayout;
    layout->addWidget(createCellWidget(tr("Process"),  DiagramItem::Process),  0, 0);
    layout->addWidget(createCellWidget(tr("Desicion"), DiagramItem::Desicion), 0, 1);
    layout->addWidget(createCellWidget(tr("InOut"),    DiagramItem::InOut),    1, 0);

    layout->setRowStretch(3, 10);
    layout->setColumnStretch(2, 10);

    QWidget* itemWidget = new QWidget;
    itemWidget->setLayout(layout);

    toolBox_ = new QToolBox;
    toolBox_->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Ignored));
    toolBox_->setMinimumWidth(itemWidget->sizeHint().width()); //! try without sizeHint()
    toolBox_->addItem(itemWidget, tr("Diagram Items"));
}

QWidget* MainWindow::createCellWidget(const QString &text, DiagramItem::DiagramType type)
{
    QIcon icon;
    switch (type) {
    case DiagramItem::Terminal:
        break;
    case DiagramItem::Process:
    case DiagramItem::Desicion:
    case DiagramItem::InOut:
        icon.addPixmap(DiagramPolygonItem(DiagramPolygonItem::DiagramPolygonType(type)).image());
        break;
    }

    QToolButton* button = new QToolButton;
    button->setIcon(icon);
    button->setIconSize(QSize(50, 50));
    button->setCheckable(true);
    buttonGroup_->addButton(button, int(type));
    QGridLayout* layout = new QGridLayout;
    layout->addWidget(button, 0, 0, Qt::AlignHCenter);
    layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);

    QWidget* widget = new QWidget;
    widget->setLayout(layout);

    return widget;
}
