#include "mainwindow.h"
#include "constants.h"
#include "internal.h"

#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , scene_(new DiagramScene(this))
    , view_(new DiagramView(scene_))
{
    createActions();
    createSideMenu();
    createMenus();

    scene_->setSceneRect(QRectF(0, 0,
                                Constants::DiagramScene::A4Width,
                                Constants::DiagramScene::A4Height));

    connect(view_,  &DiagramView::rubberBandSelectingFinished,
            scene_, &DiagramScene::selectAndMakeGroup);

    connect(view_, &DiagramView::saveFileDropped,
            this,  &MainWindow::loadFromSaveFile);

    connect(scene_, &DiagramScene::changed,
            view_,  &DiagramView::updateDiagramCountInfoTextArea);

    connect(view_,  &DiagramView::copyActionTriggered,
            scene_, &DiagramScene::copyItems);

    connect(view_,  &DiagramView::pasteActionTriggered,
            scene_, &DiagramScene::pasteItems);

    connect(view_,  &DiagramView::deleteActionTriggered,
            scene_, &DiagramScene::deleteItems);

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

    const DiagramItem::DiagramType diagramType =
            DiagramItem::DiagramType(buttonGroup_->id(button));

    DiagramItem* diagramItem = scene_->createDiagramItem(diagramType);

    scene_->addItem(diagramItem);
}

void MainWindow::onSaveAsJson()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save diagram"), tr("diagram"),
                                                    tr("JSON file (*.json)"));
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly))
        return;

    QList<ItemProperties> itemsProperties =
            internal::getDiagramItemsProperties(scene_->getDiagramItems());

    QJsonObject jsonObject = getJsonFromItemsProperties(itemsProperties);

    file.write(QJsonDocument(jsonObject).toJson(QJsonDocument::Indented));
    file.close();
}

void MainWindow::onOpenDiagram()
{
    QMessageBox::StandardButton reply =
            QMessageBox::warning(this, tr("Warning"),
                                 tr("Are you sure you want to open another flowchart?"
                                    "\nThe current flowchart will be lost."),
                                 QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open diagram"),
                                                        QDir::currentPath(),
                                                        tr("JSON files (*.json)"));
        loadFromSaveFile(fileName);
    }
}

void MainWindow::loadFromSaveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly))
        return;

    QByteArray saveData = file.readAll();
    QJsonObject jsonObject(QJsonDocument::fromJson(saveData).object());
    QList<ItemProperties> itemsProperties = getItemsPropertiesFromJson(jsonObject);

    scene_->clearScene();
    for (const auto& itemProperties : qAsConst(itemsProperties)) {
        scene_->addItem(scene_->createDiagramItem(itemProperties));
    }
    file.close();
}

void MainWindow::createSideMenu()
{
    buttonGroup_ = new QButtonGroup(this);
    buttonGroup_->setExclusive(false);
    connect(buttonGroup_, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked),
            this, &MainWindow::buttonGroupClicked);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);

    layout->addWidget(createSideMenuDiagramButton(tr("Terminal"),      DiagramItem::Terminal) );
    layout->addWidget(createSideMenuDiagramButton(tr("Process"),       DiagramItem::Process)  );
    layout->addWidget(createSideMenuDiagramButton(tr("Desicion"),      DiagramItem::Desicion) );
    layout->addWidget(createSideMenuDiagramButton(tr("In/Out"),        DiagramItem::InOut)    );
    layout->addWidget(createSideMenuDiagramButton(tr("For loop"),      DiagramItem::ForLoop)  );

    layout->setSpacing(0);

    sideMenu_ = new QScrollArea;
    sideMenu_->setObjectName("sideMenu");
    sideMenu_->setSizePolicy(QSizePolicy(QSizePolicy::Fixed,
                                         QSizePolicy::QSizePolicy::Expanding));
    sideMenu_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    sideMenu_->horizontalScrollBar()->setEnabled(false);

    QWidget* widget = new QWidget;
    widget->setLayout(layout);

    sideMenu_->setWidget(widget);
}

void MainWindow::createActions()
{
    deleteAction_ = new QAction(tr("&Delete"), this);
    deleteAction_->setShortcut(QKeySequence::Delete);
    connect(deleteAction_, &QAction::triggered, scene_, &DiagramScene::deleteSelectedItems);

    selectAllAction_ = new QAction(tr("Select &All"), this);
    selectAllAction_->setShortcut(QKeySequence::SelectAll);
    connect(selectAllAction_, &QAction::triggered, scene_, &DiagramScene::selectAllItems);

    copyAction_ = new QAction(tr("&Copy"), this);
    copyAction_->setShortcut(QKeySequence::Copy);
    connect(copyAction_, &QAction::triggered, scene_, &DiagramScene::copySelectedItems);

    pasteAction_ = new QAction(tr("&Paste"), this);
    pasteAction_->setShortcut(QKeySequence::Paste);
    connect(pasteAction_, &QAction::triggered, scene_, &DiagramScene::pasteItemsToMousePos);

    saveAsJsonAction_ = new QAction(tr("&Save Diagram"), this);
    saveAsJsonAction_->setShortcut(QKeySequence::Save);
    connect(saveAsJsonAction_, &QAction::triggered, this, &MainWindow::onSaveAsJson);

    openDiagramAction_ = new QAction(tr("&Open Diagram"), this);
    openDiagramAction_->setShortcut(QKeySequence::Open);
    connect(openDiagramAction_, &QAction::triggered, this, &MainWindow::onOpenDiagram);

    aboutQtAction_ = new QAction(tr("About &Qt"), this);
    connect(aboutQtAction_, &QAction::triggered, qApp, &QApplication::aboutQt);
}

void MainWindow::createMenus()
{
    fileMenu_ = menuBar()->addMenu(tr("&File"));
    fileMenu_->addAction(saveAsJsonAction_);
    fileMenu_->addAction(openDiagramAction_);

    itemMenu_ = menuBar()->addMenu(tr("&Item"));
    itemMenu_->addAction(deleteAction_);
    itemMenu_->addAction(selectAllAction_);
    itemMenu_->addAction(copyAction_);
    itemMenu_->addAction(pasteAction_);

    helpMenu_ = menuBar()->addMenu(tr("&Help"));
    helpMenu_->addAction(aboutQtAction_);
}

QToolButton* MainWindow::createSideMenuDiagramButton(const QString &text,
                                                     DiagramItem::DiagramType type)
{
    QToolButton* button = createSideMenuButton(text, DiagramItem::image(type));
    button->setToolTip(DiagramItem::getToolTip(type));

    buttonGroup_->addButton(button, int(type));

    return button;
}

QToolButton *MainWindow::createSideMenuButton(const QString &text, const QPixmap &pixmap)
{
    QToolButton* button = new QToolButton;
    button->setObjectName("sideMenuButton");
    button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    button->setCheckable(true);
    button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    QIcon icon(pixmap);
    button->setIcon(icon);
    button->setText(text);

    return button;
}
