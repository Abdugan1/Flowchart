#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QDebug>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
#ifdef Q_OS_ANDROID
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QApplication app(argc, argv);
    QFontDatabase::addApplicationFont(":/fonts/Montserrat-Regular.ttf");

    QFile file(":/qss/style.qss");
    file.open(QFile::ReadOnly);
    qApp->setStyleSheet(QLatin1String(file.readAll()));

    MainWindow mainWindow;
    mainWindow.resize(800, 500);
    mainWindow.show();

    return app.exec();
}
