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
        menu_entry(tr("New Workspace"),     tr("Ctrl+Shift+N"), this, [&]{ this->newWorkspace(); },   QIcon::fromTheme("document-new")),
        menu_entry(tr("Open Workspace"),    tr("Ctrl+O"),       this, [&]{ this->openWorkspace(); },  QIcon::fromTheme("document-open")),
        menu_entry(tr("Close Workspace"),   tr("Ctrl+Shift+W"), this, [&]{ this->closeWorkspace(); }, false),
        menu_entry(),
        menu_entry(tr("Save current workspace"),  tr("Ctrl+S"),       this, [&]{ },                   QIcon::fromTheme("document-save")),
        menu_entry(tr("Save to new workspace"),   tr("Ctrl+Shift+S"), this, [&]{ },                   QIcon::fromTheme("document-save-as")),
        menu_entry(),
        menu_entry(tr("&Quit"),             tr("Ctrl+Q"),       this, [&]{ this->close(); },          QIcon::fromTheme("application-exit")));
    menu_creator::add_menu(this->_menuBar, this->_fileMenu);

    // create workbench repl submenu
    this->_workbenchMenu_replSubMenu = menu_creator::make_menu(tr("Scripting Console"),
        menu_entry(tr("Open C++ Console"),        {}, this, [&]{ }),
        menu_entry(tr("Open JavaScript Console"), {}, this, [&]{ }));

    // create workbench menu
    this->_workbenchMenu = menu_creator::make_menu(tr("&Workbench"),
        menu_entry(tr("New Request"),       tr("Ctrl+N"),       this, [&]{ }, false),
        menu_entry(),
        menu_entry(this->_workbenchMenu_replSubMenu),
        menu_entry(tr("Event Log"),         {},                 this, [&]{ }));
    menu_creator::add_menu(this->_menuBar, this->_workbenchMenu);

    // create view menu
    this->_viewMenu = menu_creator::make_menu(tr("&View"),
        menu_entry(tr("Event Log"),         {},                 this, [&]{ }, true, true));
    menu_creator::add_menu(this->_menuBar, this->_viewMenu);

    // create help menu
    this->_helpMenu = menu_creator::make_menu(tr("&Help"),
        menu_entry(tr("About"),             {},                 this, [&]{ }),
        menu_entry(tr("About Qt"),          {},                 this, [&]{ qApp->aboutQt(); }));
    menu_creator::add_menu(this->_menuBar, this->_helpMenu);

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
