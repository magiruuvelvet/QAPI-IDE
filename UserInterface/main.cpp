#include <QApplication>

#include <ui/constants.hpp>

#include <windows/MainWindow.hpp>

int main(int argc, char **argv)
{
    QApplication a(argc, argv);
    a.setApplicationName(app::name.data());
    a.setApplicationDisplayName(app::prettyName.data());
    a.setApplicationVersion(app::version.data());

    MainWindow mainWindow = MainWindow();
    mainWindow.show();

    return a.exec();
}
