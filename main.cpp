#include "widget.h"
#include"drawer.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Drawer draw;
	draw.resize(250, 700);
	draw.show();
    return a.exec();
}
