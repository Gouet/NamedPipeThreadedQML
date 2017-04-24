#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "namedpipe.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qmlRegisterType<NamedPipe>("com.namedPipe", 1, 0, "NamedPipe");

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}

//#include <iostream>

//int main(int argc, char *argv[])
//{
//    HANDLE hPipe;
//    DWORD dwWritten;


//    hPipe = CreateFile(TEXT("\\\\.\\pipe\\Pipe"),
//                       GENERIC_READ | GENERIC_WRITE,
//                       0,
//                       NULL,
//                       OPEN_EXISTING,
//                       0,
//                       NULL);
//    if (hPipe != INVALID_HANDLE_VALUE)
//    {
//        std::cout << "enter here" << std::endl;
//        WriteFile(hPipe,
//                  "Hello Pipe\n",
//                  12,   // = length of string + terminating '\0' !!!
//                  &dwWritten,
//                  NULL);

//        CloseHandle(hPipe);
//    }

//    return (0);

//}
