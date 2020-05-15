#include "mainwindow.h"

#include <QApplication>
#include <ctime>

int main(int argc, char *argv[])
{
    srand(time(NULL)); //used for initializing a random starter state for tiles
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
