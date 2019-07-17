#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <memory>

#include <QWidget>
#include <QBoxLayout>
#include <QMenuBar>

#include <widgets/StatusBar.hpp>

struct menu;

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

protected:
    void closeEvent(QCloseEvent *event) override;
    //void keyPressEvent(QKeyEvent *event) override;

private:
    void newWorkspace();
    void openWorkspace();
    void closeWorkspace();

    void viewEventLogToggled(bool checked);

private:
    std::unique_ptr<QVBoxLayout> _rootLayout;
    std::unique_ptr<StatusBar> _statusBar;

    std::unique_ptr<QMenuBar> _menuBar;
    std::shared_ptr<menu> _fileMenu;
    std::shared_ptr<menu> _workbenchMenu;
    std::shared_ptr<menu> _workbenchMenu_replSubMenu;
    std::shared_ptr<menu> _viewMenu;
    std::shared_ptr<menu> _helpMenu;
};

#endif // MAINWINDOW_HPP
