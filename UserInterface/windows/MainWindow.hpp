#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <memory>

#include <QWidget>
#include <QBoxLayout>
#include <QMenuBar>

struct menu;

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    std::unique_ptr<QHBoxLayout> _rootLayout;

    std::unique_ptr<QMenuBar> _menuBar;
    std::shared_ptr<menu> _fileMenu;
};

#endif // MAINWINDOW_HPP
