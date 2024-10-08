// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "segment.h"
#include "diagramitem.h"
#include "diagramscene.h"
#include "diagramtextitem.h"
#include "mainwindow.h"

#include <QtWidgets>
#include <QScreen> 
#include <QCoreApplication>


const int InsertTextButton = 10;
const int InsertSegmentButton = 11;
const int InsertTrainButton = 12;
const int InsertLightButton = 13;

static MainWindow * MainWindow_instance = 0;


bool cpp_sim_create_item(item_t * it)
{
    if (MainWindow_instance)
        return MainWindow_instance->SimCreateItem(it);
    return false;
}

bool cpp_sim_cmd_to_ui(const char * cmd)
{
    if (MainWindow_instance)
        return MainWindow_instance->SimCmdToUI(cmd);
    return false;   
}

bool cpp_sim_create_segment(segment_t * seg)
{
    if (MainWindow_instance)
        return MainWindow_instance->SimCreateSegment(seg);
    return false;
}

bool cpp_sim_create_text(text_t * txt)
{
    if (MainWindow_instance)
        return MainWindow_instance->SimCreateText(txt);
    return false;
}

bool cpp_sim_create_train(train_t * train)
{
    if (MainWindow_instance)
        return MainWindow_instance->SimCreateTrain(train);
    return false;
}

// Must match with defs in sim_classes.py
//########### Cammnads to UI ############
#define CMD_TRAIN_PRESENT       "CMD_TRAIN_PRESENT"
#define CMD_TRAIN_UNPRESENT     "CMD_TRAIN_UNPRESENT"
#define CMD_LIGHT_GREEN         "CMD_LIGHT_GREEN"
#define CMD_LIGHT_RED           "CMD_LIGHT_RED"
#define CMD_MESSAGE_DONE        "CMD_MESSAGE_DONE"
#define CMD_SEGMENT_RED         "CMD_SEGMENT_RED"

bool MainWindow::SimCmdToUI(const char * _cmd)
{
    QString cmd_str = _cmd;
    QStringList cmd = cmd_str.split( " " );
    int id = cmd[1].toInt();

    if (cmd[0] == CMD_TRAIN_PRESENT) {
        scene->cmd_for_segment(CMD_SEGMENT_TRAIN_PRESENT, id);
    }
    else if (cmd[0] == CMD_TRAIN_UNPRESENT) {
        scene->cmd_for_segment(CMD_SEGMENT_TRAIN_UNPRESENT, id);
    }
    else if (cmd[0] == CMD_LIGHT_GREEN) {
        int pos = cmd[2].toInt();
        scene->cmd_for_segment(CMD_SEGMENT_LIGHT_GREEN, id, pos);
    }
    else if (cmd[0] == CMD_LIGHT_RED) {
        int pos = cmd[2].toInt();
        scene->cmd_for_segment(CMD_SEGMENT_LIGHT_RED, id, pos);
    }
    else if (cmd[0] == CMD_SEGMENT_RED) {
        int pos = cmd[2].toInt();
        scene->cmd_for_segment(CMD_SEGMENT_INT_RED, id, pos);
    }

    else if (cmd[0] == CMD_MESSAGE_DONE) {
        QString msg;
        for (int i = 2; i < cmd.size(); i++) {
            msg += cmd[i];
            msg += " ";
        }

        QMessageBox msgBox( (QMessageBox::Icon) id, //QMessageBox::Icon icon, 
                    "TrackSim", //const QString &title, 
                    msg); //const QString &text, 
                    //QMessageBox::StandardButtons buttons = NoButton, 
                    //QWidget *parent = nullptr, 
                    //Qt::WindowFlags f = Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint)
        msgBox.exec();

        stopRequest = true; // we can not just call stop() as we need to return to python
    }


    else {
        qWarning() << "Error: Unknown command " << cmd[0];
    }

    return true;
}

bool MainWindow::SimCreateItem(item_t * it)
{
    QPoint p(it->pos_x , it->pos_y);
    QColor color;
    color.setRgb(it->color_r, it->color_g, it->color_b);

    scene->AddItem( DiagramItem::TrackPoint,
                            scene->m_ItemMenu,
                            p,
                            color,
                            it->sim_id);
    return true;
}

bool MainWindow::SimCreateSegment(segment_t * seg)
{
    scene->AddSegment(seg);
    return true;
}

bool MainWindow::SimCreateText(text_t * txt)
{
    scene->addText(txt);
    return true;
}

bool MainWindow::SimCreateTrain(train_t * train)
{
    scene->addTrain(train);
    return true;
}

bool MainWindow::startPython()
{
    QStringList args = QCoreApplication::arguments();
    return simInt_init(args[0].toStdString().c_str());
}

void MainWindow::stopPython()
{
    simInt_destroy();
}

//! [0]
MainWindow::MainWindow() :
    timer_initted(false),
    timerIsRunning(false)
{
    MainWindow_instance = this;
    simSpeedDiv = 1;

    createActions();
    createToolBox();
    createMenus();

    scene = new DiagramScene(itemMenu, trainMenu, this);
    scene->setSceneRect(QRectF(0, 0, 5000, 5000));
    connect(scene, &DiagramScene::itemInserted,
            this, &MainWindow::itemInserted);
    connect(scene, &DiagramScene::textInserted,
            this, &MainWindow::textInserted);
    connect(scene, &DiagramScene::itemSelected,
            this, &MainWindow::itemSelected);
    createToolbars();

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(toolBox);
    view = new QGraphicsView(scene);
    layout->addWidget(view);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);

    setCentralWidget(widget);
    setWindowTitle(tr("TrackSim - Ecobee Case-Study"));
    setUnifiedTitleAndToolBarOnMac(true);
    centerAndResize();
}

void MainWindow::backgroundButtonGroupClicked(QAbstractButton *button)
{
    const QList<QAbstractButton *> buttons = backgroundButtonGroup->buttons();
    for (QAbstractButton *bt : buttons) {
        if (bt != button)
            button->setChecked(false);
    }
    QString text = button->text();
    if (text == tr("Blue Grid"))
        scene->setBackgroundBrush(QPixmap(":/images/background1.png"));
    else if (text == tr("White Grid"))
        scene->setBackgroundBrush(QPixmap(":/images/background2.png"));
    else if (text == tr("Gray Grid"))
        scene->setBackgroundBrush(QPixmap(":/images/background3.png"));
    else
        scene->setBackgroundBrush(QPixmap(":/images/background4.png"));

    scene->update();
    view->update();
}

void MainWindow::buttonGroupClicked(QAbstractButton *button)
{
    if (gMode == EditingTrain) {
        return;
    }
    if (gMode == Simulating) {
        return;
    }

    const QList<QAbstractButton *> buttons = buttonGroup->buttons();
    for (QAbstractButton *bt : buttons) {
        if (bt != button)
            button->setChecked(false);
    }
    const int id = buttonGroup->id(button);

    switch(id) {
        case InsertTextButton:
            scene->setMode(DiagramScene::InsertText);
            break;
        case InsertSegmentButton:
        case InsertTrainButton:
        case InsertLightButton:
            scene->setMode(DiagramScene::InsertTrain);
            break;
        default:
            scene->setItemType(DiagramItem::DiagramType(id));
            scene->setMode(DiagramScene::InsertItem);
            break;
    }

}

void MainWindow::saveItems()
{
    auto fileName = QFileDialog::getSaveFileName(this,
    tr("Save Railway"), "./", tr("Railway Files (*.rlw)"), 0, QFileDialog::DontUseNativeDialog);

    if (!startPython()) {
        qWarning() << "Could not start python";
        return;
    }
    scene->sendItemsToSim();

    simInt_save(fileName.toStdString().c_str());

    stopPython();
}

void MainWindow::loadItems()
{
    scene->clear();
    //invalidate();
    auto fileName = QFileDialog::getOpenFileName(this,
    tr("Load Railway"), "./", tr("Railway Files (*.rlw)"), 0, QFileDialog::DontUseNativeDialog);
    //scene->loadItems(fileName); //.toStdString());
    if (!startPython()) {
        qWarning() << "Could not start python";
        return;
    }
    simInt_load(fileName.toStdString().c_str());
    stopPython();
}

//! [3]
void MainWindow::deleteItem()
{
    QList<QGraphicsItem *> selectedItems = scene->selectedItems();
    for (QGraphicsItem *item : std::as_const(selectedItems)) {
        if (item->type() == Segment::Type) {
            scene->removeItem(item);
            Segment *segment = qgraphicsitem_cast<Segment *>(item);
            segment->startItem()->removeSegment(segment);
            segment->endItem()->removeSegment(segment);
            delete item;
        }
    }

    selectedItems = scene->selectedItems();
    for (QGraphicsItem *item : std::as_const(selectedItems)) {
         if (item->type() == DiagramItem::Type)
             qgraphicsitem_cast<DiagramItem *>(item)->removeSegments();
         scene->removeItem(item);
         delete item;
     }
}
//! [3]

//! [4]
void MainWindow::pointerGroupClicked()
{
    scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
}
//! [4]

//! [5]
void MainWindow::bringToFront()
{
    if (scene->selectedItems().isEmpty())
        return;

    QGraphicsItem *selectedItem = scene->selectedItems().first();
    const QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;
    for (const QGraphicsItem *item : overlapItems) {
        if (item->zValue() >= zValue && item->type() == DiagramItem::Type)
            zValue = item->zValue() + 0.1;
    }
    selectedItem->setZValue(zValue);
}
//! [5]

//! [6]
void MainWindow::sendToBack()
{
    if (scene->selectedItems().isEmpty())
        return;

    QGraphicsItem *selectedItem = scene->selectedItems().first();
    const QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;
    for (const QGraphicsItem *item : overlapItems) {
        if (item->zValue() <= zValue && item->type() == DiagramItem::Type)
            zValue = item->zValue() - 0.1;
    }
    selectedItem->setZValue(zValue);
}
//! [6]

//! [7]
void MainWindow::itemInserted(DiagramItem *item)
{
    pointerTypeGroup->button(int(DiagramScene::MoveItem))->setChecked(true);
    scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
    buttonGroup->button(int(item->diagramType()))->setChecked(false);
}
//! [7]

//! [8]
void MainWindow::textInserted(QGraphicsTextItem *)
{
    buttonGroup->button(InsertTextButton)->setChecked(false);
    scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
}
//! [8]

//! [9]
void MainWindow::currentFontChanged(const QFont &)
{
    handleFontChange();
}
//! [9]

//! [10]
void MainWindow::fontSizeChanged(const QString &)
{
    handleFontChange();
}
//! [10]

//! [11]
void MainWindow::sceneScaleChanged(const QString &scale)
{
    double newScale = scale.left(scale.indexOf(tr("%"))).toDouble() / 100.0;
    QTransform oldMatrix = view->transform();
    view->resetTransform();
    view->translate(oldMatrix.dx(), oldMatrix.dy());
    view->scale(newScale, newScale);
}
//! [11]

void MainWindow::simSpeedChanged(const QString &speed)
{
    if (speed == "x1") {
        simSpeedDiv = 1;
    }
    else if (speed == "x2") {
        simSpeedDiv = 2;
    }
    else if (speed == "x4") {
        simSpeedDiv = 4;
    }
    else if (speed == "x8") {
        simSpeedDiv = 8;
    }
}

//! [12]
void MainWindow::textColorChanged()
{
    textAction = qobject_cast<QAction *>(sender());
    fontColorToolButton->setIcon(createColorToolButtonIcon(
                                     ":/images/textpointer.png",
                                     qvariant_cast<QColor>(textAction->data())));
    textButtonTriggered();
}
//! [12]

//! [13]
void MainWindow::itemColorChanged()
{
    fillAction = qobject_cast<QAction *>(sender());
    fillColorToolButton->setIcon(createColorToolButtonIcon(
                                     ":/images/floodfill.png",
                                     qvariant_cast<QColor>(fillAction->data())));
    fillButtonTriggered();
}
//! [13]

//! [14]
void MainWindow::lineColorChanged()
{
    lineAction = qobject_cast<QAction *>(sender());
    lineColorToolButton->setIcon(createColorToolButtonIcon(
                                     ":/images/linecolor.png",
                                     qvariant_cast<QColor>(lineAction->data())));
    lineButtonTriggered();
}
//! [14]

//! [15]
void MainWindow::textButtonTriggered()
{
    scene->setTextColor(qvariant_cast<QColor>(textAction->data()));
}
//! [15]

//! [16]
void MainWindow::fillButtonTriggered()
{
    scene->setItemColor(qvariant_cast<QColor>(fillAction->data()));
}
//! [16]

//! [17]
void MainWindow::lineButtonTriggered()
{
    scene->setLineColor(qvariant_cast<QColor>(lineAction->data()));
}
//! [17]

//! [18]
void MainWindow::handleFontChange()
{
    QFont font = fontCombo->currentFont();
    font.setPointSize(fontSizeCombo->currentText().toInt());
    font.setWeight(boldAction->isChecked() ? QFont::Bold : QFont::Normal);
    font.setItalic(italicAction->isChecked());
    font.setUnderline(underlineAction->isChecked());

    scene->setFont(font);
}
//! [18]

//! [19]
void MainWindow::itemSelected(QGraphicsItem *item)
{
    DiagramTextItem *textItem =
    qgraphicsitem_cast<DiagramTextItem *>(item);

    QFont font = textItem->font();
    fontCombo->setCurrentFont(font);
    fontSizeCombo->setEditText(QString().setNum(font.pointSize()));
    boldAction->setChecked(font.weight() == QFont::Bold);
    italicAction->setChecked(font.italic());
    underlineAction->setChecked(font.underline());
}
//! [19]

//! [20]
void MainWindow::about()
{
    QMessageBox::about(this, tr("TrackSim"),
                       tr("The <b>Tracks Simulator</b><br>\n"
                          "\nBy: Marcelo Varanda."));

    // test text
    // startPython();
    // text_t txt;
    // memset(&txt, 0, sizeof(txt));
    // strncpy(txt.text, "Test Text !!!", sizeof(txt.text) - 1);
    // strncpy(txt.font_name, "Sans", sizeof(txt.font_name) - 1);
    // txt.size =  9;
    // txt.pos_x =  2356;
    // txt.pos_y =  2544;
    // txt.color_r =  0;
    // txt.color_g =  0;
    // txt.color_b =  0;
    // simInt_addText(&txt);
    // stopPython();

}
//! [20]

//! [21]
void MainWindow::createToolBox()
{
    buttonGroup = new QButtonGroup(this);
    buttonGroup->setExclusive(false);
    connect(buttonGroup, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked),
            this, &MainWindow::buttonGroupClicked);
    QGridLayout *layout = new QGridLayout;
//    layout->addWidget(createCellWidget(tr("Conditional"), DiagramItem::Conditional), 1, 1);
//    layout->addWidget(createCellWidget(tr("Process"), DiagramItem::Step),2, 0);
//    layout->addWidget(createCellWidget(tr("Input/Output"), DiagramItem::Io), 1, 0);
    layout->addWidget(createCellWidget(tr("Track Point"), DiagramItem::TrackPoint), 0, 1);
//! [21]

    QToolButton *textButton = new QToolButton;
    textButton->setCheckable(true);
    buttonGroup->addButton(textButton, InsertTextButton);
    textButton->setIcon(QIcon(QPixmap(":/images/textpointer.png")));
    textButton->setIconSize(QSize(50, 50));
    QGridLayout *textLayout = new QGridLayout;
    textLayout->addWidget(textButton, 0, 0, Qt::AlignHCenter);
    textLayout->addWidget(new QLabel(tr("Text")), 1, 0, Qt::AlignCenter);
    QWidget *textWidget = new QWidget;
    textWidget->setLayout(textLayout);
    layout->addWidget(textWidget, 0, 0);

    //------------ Segment Widget -------------
    // QToolButton *segmentButton = new QToolButton;
    // segmentButton->setCheckable(true);
    // buttonGroup->addButton(segmentButton, InsertTextButton);
    // segmentButton->setIcon(QIcon(QPixmap(":/images/segment.png")));
    // segmentButton->setIconSize(QSize(50, 50));
    // QGridLayout *segmentLayout = new QGridLayout;
    // segmentLayout->addWidget(segmentButton, 0, 0, Qt::AlignHCenter);
    // segmentLayout->addWidget(new QLabel(tr("Segment")), 1, 0, Qt::AlignCenter);
    // QWidget *segmentWiget = new QWidget;
    // segmentWiget->setLayout(segmentLayout);
    // layout->addWidget(segmentWiget, 1, 0);

    //------------ Light Widget -------------
    QToolButton *lightButton = new QToolButton;
    lightButton->setCheckable(true);
    buttonGroup->addButton(lightButton, InsertLightButton);
    lightButton->setIcon(QIcon(QPixmap(":/images/light.png")));
    lightButton->setIconSize(QSize(50, 50));
    QGridLayout *lightLayout = new QGridLayout;
    lightLayout->addWidget(lightButton, 0, 0, Qt::AlignHCenter);
    lightLayout->addWidget(new QLabel(tr("Light")), 1, 0, Qt::AlignCenter);
    QWidget *lightWiget = new QWidget;
    lightWiget->setLayout(lightLayout);
    layout->addWidget(lightWiget, 1, 0);

    //------------ Train Widget -------------
    QToolButton *trainButton = new QToolButton;
    trainButton->setCheckable(true);
    buttonGroup->addButton(trainButton, InsertTrainButton);
    trainButton->setIcon(QIcon(QPixmap(":/images/train.png")));
    trainButton->setIconSize(QSize(50, 50));
    QGridLayout *trainLayout = new QGridLayout;
    trainLayout->addWidget(trainButton, 0, 0, Qt::AlignHCenter);
    trainLayout->addWidget(new QLabel(tr("Train")), 1, 0, Qt::AlignCenter);
    QWidget *trainWiget = new QWidget;
    trainWiget->setLayout(trainLayout);
    layout->addWidget(trainWiget, 1, 1);

    layout->setRowStretch(3, 10);
    layout->setColumnStretch(2, 10);

    QWidget *itemWidget = new QWidget;
    itemWidget->setLayout(layout);

    backgroundButtonGroup = new QButtonGroup(this);
    connect(backgroundButtonGroup, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked),
            this, &MainWindow::backgroundButtonGroupClicked);

    QGridLayout *backgroundLayout = new QGridLayout;
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("Blue Grid"),
                                                           ":/images/background1.png"), 0, 0);
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("White Grid"),
                                                           ":/images/background2.png"), 0, 1);
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("Gray Grid"),
                                                           ":/images/background3.png"), 1, 0);
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("No Grid"),
                                                           ":/images/background4.png"), 1, 1);

    backgroundLayout->setRowStretch(2, 10);
    backgroundLayout->setColumnStretch(2, 10);

    QWidget *backgroundWidget = new QWidget;
    backgroundWidget->setLayout(backgroundLayout);


//! [22]
    toolBox = new QToolBox;
    toolBox->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Ignored));
    toolBox->setMinimumWidth(itemWidget->sizeHint().width());
    toolBox->addItem(itemWidget, tr("Railway Tools"));
    toolBox->addItem(backgroundWidget, tr("Backgrounds"));
}
//! [22]
void MainWindow::reset()
{
    scene->reset_railway();
}

void MainWindow::play()
{
    pauseAction->setDisabled(false);
    stopAction->setDisabled(false);
    playAction->setDisabled(true);

    stopRequest = false;
    scene->setMode(DiagramScene::Simulating);

    scene->reset_railway();

    if (!startPython()) {
        qWarning() << "Could not start python";
        return;
    }
    scene->sendItemsToSim();

    simInt_start();

//    stopPython();

    if (timer_initted == false) {
        timer_initted = true;
        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, QOverload<>::of(&MainWindow::timerTick));
        timer->start(TIMER_TICK_PERIOD / simSpeedDiv);
    }
    else {
        timer->setInterval(TIMER_TICK_PERIOD / simSpeedDiv);
    }

    scene->invalidate();
    timerIsRunning = true;
}

void MainWindow::pause() 
{
    timerIsRunning = !timerIsRunning;
}

void MainWindow::stop()
{
    stopPython();

    timerIsRunning = false;
    pauseAction->setDisabled(true);
    stopAction->setDisabled(true);
    playAction->setDisabled(false);

    if (pauseAction->isChecked()) {
        pauseAction->toggle();
    }
    scene->setMode(DiagramScene::MoveItem);
}

void MainWindow::timerTick()
{
    if (!timerIsRunning) return;
    if (stopRequest) {
        stop();
        return;
    }
    simInt_timer_tick();
    scene->invalidate();
}

//! [23]
void MainWindow::createActions()
{
    toFrontAction = new QAction(QIcon(":/images/bringtofront.png"),
                                tr("Bring to &Front"), this);
    toFrontAction->setShortcut(tr("Ctrl+F"));
    toFrontAction->setStatusTip(tr("Bring item to front"));
    connect(toFrontAction, &QAction::triggered, this, &MainWindow::bringToFront);

    resetAction = new QAction(QIcon(":/images/clear.png"), tr("Start Simulation"), this);
    resetAction->setShortcut(tr("Ctrl+R"));
    resetAction->setStatusTip(tr("Clear Routes"));
    connect(resetAction, &QAction::triggered, this, &MainWindow::reset);

    playAction = new QAction(QIcon(":/images/play.png"), tr("Start Simulation"), this);
    playAction->setShortcut(tr("Ctrl+S"));
    playAction->setStatusTip(tr("Start Simulation"));
    connect(playAction, &QAction::triggered, this, &MainWindow::play);

    pauseAction = new QAction(QIcon(":/images/pause.png"), tr("Pause Simulation"), this);
    pauseAction->setShortcut(tr("Ctrl+S"));
    pauseAction->setStatusTip(tr("Pause Simulation"));
    connect(pauseAction, &QAction::triggered, this, &MainWindow::pause);
    pauseAction->setDisabled(true);
    pauseAction->setCheckable(true);

    stopAction = new QAction(QIcon(":/images/stop.png"), tr("Stop Simulation"), this);
    stopAction->setShortcut(tr("Ctrl+S"));
    stopAction->setStatusTip(tr("Stop Simulation"));
    connect(stopAction, &QAction::triggered, this, &MainWindow::stop);
    stopAction->setDisabled(true);

    sendBackAction = new QAction(QIcon(":/images/sendtoback.png"), tr("Send to &Back"), this);
    sendBackAction->setShortcut(tr("Ctrl+T"));
    sendBackAction->setStatusTip(tr("Send item to back"));
    connect(sendBackAction, &QAction::triggered, this, &MainWindow::sendToBack);

    deleteAction = new QAction(QIcon(":/images/delete.png"), tr("&Delete"), this);
    deleteAction->setShortcut(tr("Delete"));
    deleteAction->setStatusTip(tr("Delete item from diagram"));
    connect(deleteAction, &QAction::triggered, this, &MainWindow::deleteItem);

    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcuts(QKeySequence::Quit);
    exitAction->setStatusTip(tr("Quit Scenediagram example"));
    connect(exitAction, &QAction::triggered, this, &QWidget::close);

    // save
    saveAction = new QAction(tr("&Save"), this);
    saveAction->setShortcut(tr("S"));
    saveAction->setStatusTip(tr("Store Railway to file"));
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveItems);

    // load
    loadAction = new QAction(tr("&Load"), this);
    loadAction->setShortcut(tr("L"));
    loadAction->setStatusTip(tr("Load Railway from file"));
    connect(loadAction, &QAction::triggered, this, &MainWindow::loadItems);

    boldAction = new QAction(tr("Bold"), this);
    boldAction->setCheckable(true);
    QPixmap pixmap(":/images/bold.png");
    boldAction->setIcon(QIcon(pixmap));
    boldAction->setShortcut(tr("Ctrl+B"));
    connect(boldAction, &QAction::triggered, this, &MainWindow::handleFontChange);

    italicAction = new QAction(QIcon(":/images/italic.png"), tr("Italic"), this);
    italicAction->setCheckable(true);
    italicAction->setShortcut(tr("Ctrl+I"));
    connect(italicAction, &QAction::triggered, this, &MainWindow::handleFontChange);

    underlineAction = new QAction(QIcon(":/images/underline.png"), tr("Underline"), this);
    underlineAction->setCheckable(true);
    underlineAction->setShortcut(tr("Ctrl+U"));
    connect(underlineAction, &QAction::triggered, this, &MainWindow::handleFontChange);

    aboutAction = new QAction(tr("A&bout"), this);
    aboutAction->setShortcut(tr("F1"));
    connect(aboutAction, &QAction::triggered, this, &MainWindow::about);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(exitAction);

    fileMenu->addAction(loadAction);
    fileMenu->addAction(saveAction);

    itemMenu = menuBar()->addMenu(tr("&Item"));
    itemMenu->addAction(deleteAction);
    itemMenu->addSeparator();
    itemMenu->addAction(toFrontAction);
    itemMenu->addAction(sendBackAction);

    itemMenu->addSeparator();
    itemMenu->addAction(playAction);
    itemMenu->addAction(pauseAction);
    itemMenu->addAction(stopAction);

    aboutMenu = menuBar()->addMenu(tr("&Help"));
    aboutMenu->addAction(aboutAction);


    trainMenu = menuBar()->addMenu(tr("Train")); //myMenu.addAction("Menu Item 1");
    trainMenu->addAction(deleteAction);
}
//! [24]

//! [25]
void MainWindow::createToolbars()
{
//! [25]
    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(deleteAction);
    editToolBar->addAction(toFrontAction);
    editToolBar->addAction(sendBackAction);

    editToolBar->addAction(resetAction);
    editToolBar->addAction(playAction);
    editToolBar->addAction(pauseAction);
    editToolBar->addAction(stopAction);

    fontCombo = new QFontComboBox();
    connect(fontCombo, &QFontComboBox::currentFontChanged,
            this, &MainWindow::currentFontChanged);

    fontSizeCombo = new QComboBox;
    fontSizeCombo->setEditable(true);
    for (int i = 8; i < 30; i = i + 2)
        fontSizeCombo->addItem(QString().setNum(i));
    QIntValidator *validator = new QIntValidator(2, 64, this);
    fontSizeCombo->setValidator(validator);
    connect(fontSizeCombo, &QComboBox::currentTextChanged,
            this, &MainWindow::fontSizeChanged);

    fontColorToolButton = new QToolButton;
    fontColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    fontColorToolButton->setMenu(createColorMenu(&MainWindow::textColorChanged, Qt::black));
    textAction = fontColorToolButton->menu()->defaultAction();
    fontColorToolButton->setIcon(createColorToolButtonIcon(":/images/textpointer.png", Qt::black));
    fontColorToolButton->setAutoFillBackground(true);
    connect(fontColorToolButton, &QAbstractButton::clicked,
            this, &MainWindow::textButtonTriggered);

//! [26]
    fillColorToolButton = new QToolButton;
    fillColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    fillColorToolButton->setMenu(createColorMenu(&MainWindow::itemColorChanged, DEFAULT_ITEM_COLOR));
    fillAction = fillColorToolButton->menu()->defaultAction();
    fillColorToolButton->setIcon(createColorToolButtonIcon(
                                     ":/images/floodfill.png", DEFAULT_ITEM_COLOR));
    connect(fillColorToolButton, &QAbstractButton::clicked,
            this, &MainWindow::fillButtonTriggered);
//! [26]

    lineColorToolButton = new QToolButton;
    lineColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    lineColorToolButton->setMenu(createColorMenu(&MainWindow::lineColorChanged, Qt::black));
    lineAction = lineColorToolButton->menu()->defaultAction();
    lineColorToolButton->setIcon(createColorToolButtonIcon(
                                     ":/images/linecolor.png", Qt::black));
    connect(lineColorToolButton, &QAbstractButton::clicked,
            this, &MainWindow::lineButtonTriggered);

    textToolBar = addToolBar(tr("Font"));
    textToolBar->addWidget(fontCombo);
    textToolBar->addWidget(fontSizeCombo);
    textToolBar->addAction(boldAction);
    textToolBar->addAction(italicAction);
    textToolBar->addAction(underlineAction);

    colorToolBar = addToolBar(tr("Color"));
    colorToolBar->addWidget(fontColorToolButton);
    colorToolBar->addWidget(fillColorToolButton);
    colorToolBar->addWidget(lineColorToolButton);

    QToolButton *pointerButton = new QToolButton;
    pointerButton->setCheckable(true);
    pointerButton->setChecked(true);
    pointerButton->setIcon(QIcon(":/images/pointer.png"));

    QToolButton *linePointerButton = new QToolButton;
    linePointerButton->setCheckable(true);
    linePointerButton->setIcon(QIcon(":/images/segment.png"));
    linePointerButton->setToolTip("Segment");

    pointerTypeGroup = new QButtonGroup(this);
    pointerTypeGroup->addButton(pointerButton, int(DiagramScene::MoveItem));
    pointerTypeGroup->addButton(linePointerButton, int(DiagramScene::InsertLine));
    connect(pointerTypeGroup, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked),
            this, &MainWindow::pointerGroupClicked);

    sceneScaleCombo = new QComboBox;
    QStringList scales;
    scales << tr("50%") << tr("75%") << tr("100%") << tr("125%") << tr("150%");
    sceneScaleCombo->addItems(scales);
    sceneScaleCombo->setCurrentIndex(2);
    connect(sceneScaleCombo, &QComboBox::currentTextChanged,
            this, &MainWindow::sceneScaleChanged);

    sceneSpeedCombo = new QComboBox;
    QStringList speeds;
    speeds << tr("x1") << tr("x2") << tr("x4") << tr("x8");
    sceneSpeedCombo->addItems(speeds);
    sceneSpeedCombo->setCurrentIndex(0);
    connect(sceneSpeedCombo, &QComboBox::currentTextChanged,
            this, &MainWindow::simSpeedChanged);


    pointerToolbar = addToolBar(tr("Pointer type"));
    pointerToolbar->addWidget(pointerButton);
    pointerToolbar->addWidget(linePointerButton);
    pointerToolbar->addWidget(sceneScaleCombo);
    pointerToolbar->addWidget(sceneSpeedCombo);



//! [27]
}
//! [27]

//! [28]
QWidget *MainWindow::createBackgroundCellWidget(const QString &text, const QString &image)
{
    QToolButton *button = new QToolButton;
    button->setText(text);
    button->setIcon(QIcon(image));
    button->setIconSize(QSize(50, 50));
    button->setCheckable(true);
    backgroundButtonGroup->addButton(button);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(button, 0, 0, Qt::AlignHCenter);
    layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);

    return widget;
}
//! [28]

//! [29]
QWidget *MainWindow::createCellWidget(const QString &text, DiagramItem::DiagramType type)
{

    DiagramItem item(type, itemMenu);
    QIcon icon(item.image());

    QToolButton *button = new QToolButton;
    button->setIcon(icon);
    button->setIconSize(QSize(50, 50));
    button->setCheckable(true);
    buttonGroup->addButton(button, int(type));

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(button, 0, 0, Qt::AlignHCenter);
    layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);

    return widget;
}
//! [29]

//! [30]
template<typename PointerToMemberFunction>
QMenu *MainWindow::createColorMenu(const PointerToMemberFunction &slot, QColor defaultColor)
{
    QList<QColor> colors;
    colors << Qt::black << Qt::white << Qt::red << Qt::blue << Qt::yellow;
    QStringList names;
    names << tr("black") << tr("white") << tr("red") << tr("blue")
          << tr("yellow");

    QMenu *colorMenu = new QMenu(this);
    for (int i = 0; i < colors.count(); ++i) {
        QAction *action = new QAction(names.at(i), this);
        action->setData(colors.at(i));
        action->setIcon(createColorIcon(colors.at(i)));
        connect(action, &QAction::triggered, this, slot);
        colorMenu->addAction(action);
        if (colors.at(i) == defaultColor)
            colorMenu->setDefaultAction(action);
    }
    return colorMenu;
}
//! [30]

//! [31]
QIcon MainWindow::createColorToolButtonIcon(const QString &imageFile, QColor color)
{
    QPixmap pixmap(50, 80);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    QPixmap image(imageFile);
    // Draw icon centred horizontally on button.
    QRect target(4, 0, 42, 43);
    QRect source(0, 0, 42, 43);
    painter.fillRect(QRect(0, 60, 50, 80), color);
    painter.drawPixmap(target, image, source);

    return QIcon(pixmap);
}
//! [31]

//! [32]
QIcon MainWindow::createColorIcon(QColor color)
{
    QPixmap pixmap(20, 20);
    QPainter painter(&pixmap);
    painter.setPen(Qt::NoPen);
    painter.fillRect(QRect(0, 0, 20, 20), color);

    return QIcon(pixmap);
}
//! [32]

void MainWindow::centerAndResize() {
    // get the dimension available on this screen
    // QSize availableSize = qApp->desktop()->availableGeometry().size();
    QSize availableSize = QApplication::primaryScreen()->geometry().size();  // Qt6 , along with the #include <QScreen>
    int width = availableSize.width();
    int height = availableSize.height();
    width *= 0.8; // 80% of the screen size
    height *= 0.7; // 70% of the screen size
    setMinimumSize(width,height);
}

