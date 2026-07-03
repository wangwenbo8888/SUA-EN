#include <QApplication>
#include"clientwidget.h"

#include "log.h"

int main(int argc, char *argv[])
{
    qInstallMessageHandler(updateLog);

    QApplication a(argc, argv);
    ClientWidget w;
    w.move(0,0);

    w.show();

    return a.exec();
}
