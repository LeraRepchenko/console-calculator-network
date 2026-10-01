#include <QCoreApplication>
#include <windows.h>
#include "server.h"

int main(int argc, char *argv[])
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    QCoreApplication app(argc, argv);
    Server server(5555);
    return app.exec();
}