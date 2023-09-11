#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("复眼综合显控图(Ver 1.0)");
    w.resize(1600,900);
    w.setMinimumSize(800,600);
    w.show();
    return a.exec();
}
