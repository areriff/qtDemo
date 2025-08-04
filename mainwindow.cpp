#include "mainwindow.h"

#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QStatusBar>
#include <QToolBar>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QToolButton>
#include <QCommandLinkButton>
#include <QCheckBox>
#include <QRadioButton>
#include <QComboBox>
#include <qguiapplication.h>
#include <QProgressBar>
#include <QScrollArea>
#include <QScreen>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    setWindowTitle("qtDemo");
    QScreen *screen = QGuiApplication::primaryScreen();
    int screenWidth = 0;
    int screenHeight = 0;
    if (screen != nullptr) {
        QRect geometry = screen->geometry();
        screenWidth = geometry.width();
        screenHeight = geometry.height();
    }
    resize(screenWidth / 2, screenHeight / 2);
    setGeometry(QRect(100, 100, screenWidth - 200, screenHeight - 200));

    // MENUBAR
    QMenu *fileMenu = menuBar()->addMenu("&File");
    QAction *exitAction = new QAction("Exit", this);
    fileMenu->addAction(exitAction);
    connect(exitAction, &QAction::triggered, this, &QMainWindow::close);

    QMenu *editMenu = menuBar()->addMenu("&Edit");

    // --- Actions ---
    QAction *cutAction = editMenu->addAction("Cut");
    cutAction->setShortcut(QKeySequence::Cut);

    QAction *copyAction = editMenu->addAction("Copy");
    copyAction->setShortcut(QKeySequence::Copy);

    QAction *pasteAction = editMenu->addAction("Paste");
    pasteAction->setShortcut(QKeySequence::Paste);

    QAction *selectAllAction = editMenu->addAction("Select All");
    selectAllAction->setShortcut(QKeySequence::SelectAll);

    // --- Helper lambda: get current editable widget ---
    auto currentEditable = []() -> QWidget * {
        QWidget *w = QApplication::focusWidget();
        // Only allow QLineEdit and QTextEdit (you can add QPlainTextEdit if needed)
        if (qobject_cast<QLineEdit *>(w) || qobject_cast<QTextEdit *>(w))
            return w;
        return nullptr;
    };

    // --- Enable/disable menu items dynamically ---
    auto updateEditActions = [=]() {
        QWidget *w = QApplication::focusWidget();
        bool enabled = false;
        if (auto line = qobject_cast<QLineEdit *>(w)) {
            enabled = !line->isReadOnly();
            cutAction->setEnabled(enabled && line->hasSelectedText());
            copyAction->setEnabled(line->hasSelectedText());
            pasteAction->setEnabled(enabled);
            selectAllAction->setEnabled(true);
            return;
        }
        if (auto text = qobject_cast<QTextEdit *>(w)) {
            enabled = !text->isReadOnly();
            cutAction->setEnabled(enabled && text->textCursor().hasSelection());
            copyAction->setEnabled(text->textCursor().hasSelection());
            pasteAction->setEnabled(enabled);
            selectAllAction->setEnabled(true);
            return;
        }

        // If focus is on something else
        cutAction->setEnabled(false);
        copyAction->setEnabled(false);
        pasteAction->setEnabled(false);
        selectAllAction->setEnabled(false);
    };

    // Update menu actions every time the menu is about to open
    connect(editMenu, &QMenu::aboutToShow, this, updateEditActions);

    // --- Connect action triggers ---
    connect(cutAction, &QAction::triggered, this, [=]() {
        if (auto le = qobject_cast<QLineEdit *>(currentEditable()))
            le->cut();
        else if (auto te = qobject_cast<QTextEdit *>(currentEditable()))
            te->cut();
    });

    connect(copyAction, &QAction::triggered, this, [=]() {
        if (auto le = qobject_cast<QLineEdit *>(currentEditable()))
            le->copy();
        else if (auto te = qobject_cast<QTextEdit *>(currentEditable()))
            te->copy();
    });

    connect(pasteAction, &QAction::triggered, this, [=]() {
        if (auto le = qobject_cast<QLineEdit *>(currentEditable()))
            le->paste();
        else if (auto te = qobject_cast<QTextEdit *>(currentEditable()))
            te->paste();
    });

    connect(selectAllAction, &QAction::triggered, this, [=]() {
        if (auto le = qobject_cast<QLineEdit *>(currentEditable()))
            le->selectAll();
        else if (auto te = qobject_cast<QTextEdit *>(currentEditable()))
            te->selectAll();
    });


    // TOOLBAR
    QToolBar *toolBar = addToolBar("Main Toolbar");
    QAction *dummyAction = new QAction("Action", this);
    toolBar->addAction(dummyAction);

    // CENTRAL WIDGET
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout *mainLayout = new QVBoxLayout(central);

    // TABS
    QTabWidget *tabWidget = new QTabWidget;
    mainLayout->addWidget(tabWidget);

    // TAB 1: Basic Controls
    QWidget *tab1 = new QWidget;
    QVBoxLayout *tab1Layout = new QVBoxLayout(tab1);

    QLabel *label = new QLabel("This is a label");
    tab1Layout->addWidget(label);

    QLineEdit *lineEdit = new QLineEdit;
    lineEdit->setPlaceholderText("Enter single-line text");
    tab1Layout->addWidget(lineEdit);

    QTextEdit *textEdit = new QTextEdit;
    textEdit->setPlaceholderText("Multi-line text box");
    tab1Layout->addWidget(textEdit);

    QPushButton *pushButton = new QPushButton("Push Button");
    QToolButton *toolButton = new QToolButton;
    toolButton->setText("Tool Button");
    QCommandLinkButton *cmdButton = new QCommandLinkButton("Command Link", "Additional description");
    tab1Layout->addWidget(pushButton);
    tab1Layout->addWidget(toolButton);
    tab1Layout->addWidget(cmdButton);

    QCheckBox *checkBox = new QCheckBox("Check Me");
    tab1Layout->addWidget(checkBox);

    QRadioButton *radio1 = new QRadioButton("Option 1");
    QRadioButton *radio2 = new QRadioButton("Option 2");
    tab1Layout->addWidget(radio1);
    tab1Layout->addWidget(radio2);

    QComboBox *comboBox = new QComboBox;
    comboBox->addItems({"Item 1", "Item 2", "Item 3"});
    tab1Layout->addWidget(comboBox);

    QProgressBar *progressBar = new QProgressBar;
    progressBar->setValue(40);
    tab1Layout->addWidget(progressBar);

    tabWidget->addTab(tab1, "Controls");

    // TAB 2: Scroll Area Example
    QWidget *tab2 = new QWidget;
    QVBoxLayout *tab2Layout = new QVBoxLayout(tab2);

    QScrollArea *scrollArea = new QScrollArea;
    QWidget *scrollContent = new QWidget;
    QVBoxLayout *scrollLayout = new QVBoxLayout(scrollContent);

    for (int i = 1; i <= 40; ++i) {
        scrollLayout->addWidget(new QLabel(QString("Scrollable label %1").arg(i)));
    }

    scrollContent->setLayout(scrollLayout);
    scrollArea->setWidget(scrollContent);
    scrollArea->setWidgetResizable(true);

    tab2Layout->addWidget(scrollArea);
    tabWidget->addTab(tab2, "Scroll Area");

    // STATUS BAR
    statusBar()->showMessage("Ready");
}
