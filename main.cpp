#include "textsearch.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TextSearch w;
    w.show();
    return a.exec();
}
