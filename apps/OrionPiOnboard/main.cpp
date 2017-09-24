#include <QCoreApplication>
#include "OrionEngine.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    OrionEngine engine;
    QObject::connect(&engine, &OrionEngine::aboutToClose,
                     &a, &QCoreApplication::quit);

    return a.exec();
}
