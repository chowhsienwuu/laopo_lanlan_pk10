#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QString buildtime;
    buildtime.append("算水器,");
    buildtime.append(__DATE__);
    buildtime.append("  ");
    buildtime.append(__TIME__);
    w.setWindowTitle(buildtime);
    w.show();

    return a.exec();
}
