#include "CVDevelopWidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CVDevelopWidget w;
    w.show();
    return a.exec();
}
