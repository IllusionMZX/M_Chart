#include "mainwidget.h"

#include <QApplication>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWidget w;
    w.setWindowTitle("M_CHART");
    w.show();
    return a.exec();
}
