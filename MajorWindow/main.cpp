#include "cmajor.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    CMajor window;
    window.isLoadPlugin();

    window.show();

    return a.exec();
}
