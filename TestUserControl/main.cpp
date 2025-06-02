#include "TestUserControl.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TestUserControl w;
    w.show();
    return a.exec();
}
