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
    ~MainWindow() override;

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    void newWorkspace();
    void openWorkspace();
    void closeWorkspace();

private:
    std::unique_ptr<QHBoxLayout> _rootLayout;

    std::unique_ptr<QMenuBar> _menuBar;
    std::shared_ptr<menu> _fileMenu;
    std::shared_ptr<menu> _workbenchMenu;
    std::shared_ptr<menu> _workbenchMenu_replSubMenu;
};

#endif // MAINWINDOW_HPP
