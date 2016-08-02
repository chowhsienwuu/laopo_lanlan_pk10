#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
  //  a.seWindowIcon(QIcon(":/new/prefix1/love.ico"));
    MainWindow w;
    w.show();

    return a.exec();
}
