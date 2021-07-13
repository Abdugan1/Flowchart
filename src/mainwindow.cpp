#include "mainwindow.h"
#include "pathresizer.h"

#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , scene_(new DiagramScene(this))
    , view_(new DiagramView(scene_))
{
    createActions();
    createSideMenu();
    createMenus();

    scene_->setSceneRect(QRectF(0, 0, DiagramScene::Width, DiagramScene::Height));
    scene_->addRect(QRectF(0, 0, DiagramScene::Width, DiagramScene::Height));

    view_->setObjectName("graphicsView");
    view_->setRenderHint(QPainter::Antialiasing, true);

    connect(view_,  &DiagramView::rubberBandSelectingFinished,
            scene_, &DiagramScene::makeGroupOfSelectedItems);

    QHBoxLayout * hLayout = new QHBoxLayout;
    hLayout->setContentsMargins(0, 0, 0, 0);
    hLayout->setSpacing(0);

    hLayout->addWidget(sideMenu_);
    hLayout->addWidget(view_);

    QWidget* widget = new QWidget;
    widget->setLayout(hLayout);
    setCentralWidget(widget);
}

void MainWindow::buttonGroupClicked(QAbstractButton* button)
{
    const QList<QAbstractButton*> buttons = buttonGroup_->buttons();
    for (auto myButton : buttons) {
        if (myButton != button)
            button->setChecked(false);
    }

    const DiagramItem::DiagramType type =
            DiagramItem::DiagramType(buttonGroup_->id(button));
    DiagramItem* diagramItem = new DiagramItem(DiagramItem::DiagramType(type));

    SizeGripItem* sizeGripItem =
            new SizeGripItem(new PathResizer, diagramItem);
    connect(sizeGripItem, &SizeGripItem::itemResized,
            diagramItem,  &DiagramItem::updateTextItemPosition);


    connect(diagramItem, &DiagramItem::itemPositionChanged,
            scene_,      &DiagramScene::onItemPositionChanged);

    connect(diagramItem, &DiagramItem::itemReleased,
            scene_,      &DiagramScene::onItemReleased);

    scene_->addItem(diagramItem);
}

void MainWindow::createSideMenu()
{
    buttonGroup_ = new QButtonGroup(this);
    buttonGroup_->setExclusive(false);
    connect(buttonGroup_, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked),
            this, &MainWindow::buttonGroupClicked);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);

    layout->addWidget(createSideMenuButton(tr("Terminal"),      DiagramItem::Terminal) );
    layout->addWidget(createSideMenuButton(tr("Process"),       DiagramItem::Process)  );
    layout->addWidget(createSideMenuButton(tr("Desicion"),      DiagramItem::Desicion) );
    layout->addWidget(createSideMenuButton(tr("In/Out"),        DiagramItem::InOut)    );
    layout->addWidget(createSideMenuButton(tr("For loop"),      DiagramItem::ForLoop)  );

    layout->setSpacing(0);
    QSpacerItem* spacer = new QSpacerItem(20, 40, QSizePolicy::Minimum,
                                                  QSizePolicy::Expanding);
    layout->addItem(spacer);

    sideMenu_ = new QFrame;
    sideMenu_->setObjectName("sideMenu");
    sideMenu_->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Ignored));
    sideMenu_->setLayout(layout);
}

void MainWindow::createActions()
{
    deleteAction_ = new QAction(tr("&Delete"), this);
    deleteAction_->setShortcut(tr("Delete"));
    connect(deleteAction_, &QAction::triggered, scene_, &DiagramScene::deleteSelectedItems);

    selectAllAction_ = new QAction(tr("Select &All"), this);
    selectAllAction_->setShortcut(tr("Ctrl+A"));
    connect(selectAllAction_, &QAction::triggered, scene_, &DiagramScene::selectAllItems);
}

void MainWindow::createMenus()
{
    itemMenu_ = menuBar()->addMenu(tr("&Item"));
    itemMenu_->addAction(deleteAction_);
    itemMenu_->addAction(selectAllAction_);
}

QToolButton* MainWindow::createSideMenuButton(const QString &text, DiagramItem::DiagramType type)
{
    QIcon icon;
    icon.addPixmap(DiagramItem(DiagramItem::DiagramType(type)).image());

    QToolButton* button = new QToolButton;
    button->setObjectName("sideMenuButton");
    button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    button->setCheckable(true);
    button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    button->setIcon(icon);
    button->setText(text);

    buttonGroup_->addButton(button, int(type));

    return button;
}
