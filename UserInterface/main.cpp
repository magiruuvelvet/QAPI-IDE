#include <QApplication>
#include <QTranslator>

#include <ui/constants.hpp>

#include <logger/log.hpp>

#include <windows/MainWindow.hpp>

int main(int argc, char **argv)
{
    named_thread::set_name(std::this_thread::get_id(), "main");

    QApplication a(argc, argv);
    a.setApplicationName(app::name.data());
    a.setApplicationDisplayName(app::prettyName.data());
    a.setApplicationVersion(app::version.data());

    // load translations for current locale from embedded qm files
    // if no translation was found, the builtin English strings are used instead
    QTranslator translator;
    translator.load(QLocale(), {}, {}, ":");
    a.installTranslator(&translator);

    MainWindow mainWindow = MainWindow();
    mainWindow.show();

    return a.exec();
}
