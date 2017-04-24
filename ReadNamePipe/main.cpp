#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <strsafe.h>
#include <iostream>

#include "namedpipe.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qmlRegisterType<NamedPipe>("com.namedPipe", 1, 0, "NamedPipe");

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    //NamedPipe namepipe("\\\\.\\pipe\\Pipe", "\\\\.\\pipe\\Pipe2");

//    while (true);

//    return;
//    HANDLE hPipe;
//    char buffer[1024];
//    DWORD dwRead;


//    std::cout << "lolilol" << std::endl;
//  //  printf("and here ?\n");
//        hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\Pipe"),
//                                PIPE_ACCESS_DUPLEX | PIPE_TYPE_BYTE | PIPE_READMODE_BYTE,   // FILE_FLAG_FIRST_PIPE_INSTANCE is not needed but forces CreateNamedPipe(..) to fail if the pipe already exists...
//                                PIPE_WAIT,
//                                1,
//                                1024 * 16,
//                                1024 * 16,
//                                NMPWAIT_USE_DEFAULT_WAIT,
//                                NULL);
//       // printf("pass ??\n");
//        while (hPipe != INVALID_HANDLE_VALUE)
//        {
//            std::cout << "wait connection !" << std::endl;
//            if (ConnectNamedPipe(hPipe, NULL) != FALSE)   // wait for someone to connect to the pipe
//            {
//                std::cout << "connection !" << std::endl;
//                while (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE)
//                {
//                    /* add terminating zero */
//                    buffer[dwRead] = '\0';

//                    /* do something with data in buffer */
//                  //  printf("%s\n", buffer);
//                    std::cout << buffer << std::endl;
//                }
//            }

//            DisconnectNamedPipe(hPipe);
//            std::cout << "disconnect !" << std::endl;
//        }


    return app.exec();
}
