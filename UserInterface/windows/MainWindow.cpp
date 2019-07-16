#include "MainWindow.hpp"

#include <utils/menu_creator.hpp>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    this->_rootLayout = std::make_unique<QHBoxLayout>();
    this->_rootLayout->setMargin(0);

    this->_menuBar = std::make_unique<QMenuBar>();
    this->_fileMenu = menu_creator::make_menu(tr("&File"),
        menu_entry("Test Entry 1", {}, this, [&]{ }),
        menu_entry("Test Entry 2", {}, this, [&]{ }),
        menu_entry("Test Entry 3", {}, this, [&]{ }),
        menu_entry("&Quit", tr("Ctrl+Q"), this, [&]{ this->close(); }));
    this->_menuBar->addMenu(this->_fileMenu->menu.get());

    // create layout
    this->_rootLayout->addWidget(this->_menuBar.get());

    this->setLayout(this->_rootLayout.get());
}

MainWindow::~MainWindow()
{
}
