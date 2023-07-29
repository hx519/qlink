#include "Mymap.h"
#include "Mymenu.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyMenu mainMenu;
    mainMenu.show();
    return a.exec();
}
