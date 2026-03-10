#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setPalette(MainWindow::setTheme(MainWindow::Light));
    MainWindow w;
    w.show();
    return a.exec();
}
