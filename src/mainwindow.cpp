#include "mainwindow.h"

#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    createActions();
    createToolBox();
    createMenus();

    scene_ = new DiagramScene(this);
    scene_->setSceneRect(QRectF(0, 0, DiagramScene::Width, DiagramScene::Height));
    scene_->addRect(QRectF(0, 0, DiagramScene::Width, DiagramScene::Height));

    view_ = new DiagramView(scene_);
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
    case DiagramItem::Process:
    case DiagramItem::Desicion:
    case DiagramItem::InOut:
        diagramItem = new DiagramItem(DiagramItem::DiagramType(type));
        break;
    }
    connect(diagramItem, &DiagramItem::itemPositionChanging,
            scene_,      &DiagramScene::onItemPositionChanging);

    connect(diagramItem, &DiagramItem::itemReleased,
            scene_,      &DiagramScene::onItemReleased);

    scene_->addItem(diagramItem);
}

void MainWindow::deleteItem()
{
    QList<QGraphicsItem*> selectedItems = scene_->selectedItems();
    for (QGraphicsItem* item : selectedItems) {
        delete item;
    }
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

void MainWindow::createActions()
{
    deleteAction_ = new QAction(tr("&Delete"), this);
    deleteAction_->setShortcut(tr("Delete"));
    connect(deleteAction_, &QAction::triggered, this, &MainWindow::deleteItem);
}

void MainWindow::createMenus()
{
    itemMenu_ = menuBar()->addMenu(tr("&Item"));
    itemMenu_->addAction(deleteAction_);
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
        icon.addPixmap(DiagramItem(DiagramItem::DiagramType(type)).image());
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
