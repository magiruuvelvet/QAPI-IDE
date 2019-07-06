#include <QApplication>

#include <windows/MainWindow.hpp>

int main(int argc, char **argv)
{
    QApplication a(argc, argv);
    a.setApplicationName("QAPI-IDE");
    a.setApplicationDisplayName("QApi IDE");
    a.setApplicationVersion("draft");

    MainWindow mainWindow = MainWindow();
    mainWindow.show();

    return a.exec();
}
