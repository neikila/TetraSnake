#include "myclient.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyClient w;
    w.show();
    w.focusPolicy();
    return a.exec();
}
