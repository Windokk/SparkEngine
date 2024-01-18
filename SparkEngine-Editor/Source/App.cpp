#include "Gui/QtApp.h"
#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    QtApp w;
    w.show();
    return a.exec();
}
