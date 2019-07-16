#include "MainWindow.hpp"

#include <QApplication>
#include <QCloseEvent>

#include <utils/menu_creator.hpp>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    this->_rootLayout = std::make_unique<QHBoxLayout>();
    this->_rootLayout->setMargin(0);

    this->_menuBar = std::make_unique<QMenuBar>();

    // create file menu
    this->_fileMenu = menu_creator::make_menu(tr("&File"),
        menu_entry(tr("New Workspace"),     tr("Ctrl+Shift+N"), this, [&]{ this->newWorkspace(); }),
        menu_entry(tr("Open Workspace"),    tr("Ctrl+O"),       this, [&]{ this->openWorkspace(); }),
        menu_entry(tr("Close Workspace"),   tr("Ctrl+Shift+W"), this, [&]{ this->closeWorkspace(); }, false),
        menu_entry(),
        menu_entry(tr("&Quit"),             tr("Ctrl+Q"),       this, [&]{ this->close(); }));
    this->_menuBar->addMenu(this->_fileMenu->menu.get());

    // create workbench repl submenu
    this->_workbenchMenu_replSubMenu = menu_creator::make_menu(tr("Repl"),
        menu_entry(tr("Open C++ Console"),        {}, this, [&]{ }),
        menu_entry(tr("Open JavaScript Console"), {}, this, [&]{ }));

    // create workbench menu
    this->_workbenchMenu = menu_creator::make_menu(tr("Workbench"),
        menu_entry(tr("New Request"),       tr("Ctrl+N"),       this, [&]{ }, false),
        menu_entry(),
        menu_entry(this->_workbenchMenu_replSubMenu.get()));
    this->_menuBar->addMenu(this->_workbenchMenu->menu.get());

    // create layout
    this->_rootLayout->addWidget(this->_menuBar.get());

    this->setLayout(this->_rootLayout.get());
}

MainWindow::~MainWindow()
{
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // quit application when main window closes
    // automatically closing all child windows
    qApp->quit();
    event->accept();
}

void MainWindow::newWorkspace()
{

}

void MainWindow::openWorkspace()
{

}

void MainWindow::closeWorkspace()
{

}
