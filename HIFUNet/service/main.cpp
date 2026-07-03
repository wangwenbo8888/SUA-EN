#include "servewidget.h"
#include <QApplication>

#include "log.h"

int main(int argc, char *argv[])
{
    qInstallMessageHandler(updateLog);

    QApplication a(argc, argv);
    ServeWidget w;
    w.show();

    return a.exec();
}
