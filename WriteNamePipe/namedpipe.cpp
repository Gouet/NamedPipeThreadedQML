#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <strsafe.h>
#include <iostream>
#include "namedpipe.h"

// NAMED PIPE

NamedPipe::NamedPipe() : QObject(0), threadRead(NULL), threadWrite(NULL) {

   // QObject::connect(threadRead, SIGNAL(receiveData(QString)), this, SIGNAL(receiveData(QString)));

   // threadRead->start();
}

NamedPipe::~NamedPipe() {

}


QString const &NamedPipe::fileIn() const {
    return (_fileIn);
}

QString const &NamedPipe::fileOut() const {
    return (_fileOut);
}

void NamedPipe::setFileIn(QString const &data) {
    _fileIn = data;
    if (!threadRead) {
        threadRead = new NamedPipeThread::NamedPipeThreadRead(_fileIn);
        QObject::connect(threadRead, SIGNAL(receiveData(QString)), this, SIGNAL(receiveData(QString)));
        threadRead->start();
    }
    emit fileInChanged();
}

void NamedPipe::setFileOut(QString const &data) {
    _fileOut = data;
    if (!threadWrite) {
        threadWrite = new NamedPipeThread::NamedPipeThreadWrite(_fileOut);
        QObject::connect(threadWrite, SIGNAL(sendData()), this, SIGNAL(sendData()));
        threadWrite->start();
    }
    emit fileOutChanged();
}


void NamedPipe::operator<<(QString const &data) {
    if (threadWrite) {
        *threadWrite << data;
    }
}

void NamedPipe::send(QString const &data) {
    (*this) << data;
}

// THREAD READ

NamedPipeThread::NamedPipeThreadRead::NamedPipeThreadRead(QString const &fileIn) : QThread(0)
    , _fileIn(fileIn) {

}

NamedPipeThread::NamedPipeThreadRead::~NamedPipeThreadRead() {
}

void NamedPipeThread::NamedPipeThreadRead::run() {
    HANDLE hPipe;
    char buffer[1024];
    DWORD dwRead;

    LPCWSTR fileToRead = (LPCWSTR)_fileIn.utf16();

        hPipe = CreateNamedPipe(fileToRead,
                                PIPE_ACCESS_DUPLEX | PIPE_TYPE_BYTE | PIPE_READMODE_BYTE,   // FILE_FLAG_FIRST_PIPE_INSTANCE is not needed but forces CreateNamedPipe(..) to fail if the pipe already exists...
                                PIPE_WAIT,
                                1,
                                1024 * 16,
                                1024 * 16,
                                NMPWAIT_USE_DEFAULT_WAIT,
                                NULL);
        while (hPipe != INVALID_HANDLE_VALUE)
        {
            if (ConnectNamedPipe(hPipe, NULL) != FALSE)   // wait for someone to connect to the pipe
            {
                while (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE)
                {
                    /* add terminating zero */
                    buffer[dwRead] = '\0';

                    emit receiveData(QString(buffer));
                }
            }

            DisconnectNamedPipe(hPipe);
            std::cout << "disconnect !" << std::endl;
        }
}

// THREAD WRITE

NamedPipeThread::NamedPipeThreadWrite::NamedPipeThreadWrite(QString const &fileOut) : QThread(0)
    , _fileOut(fileOut), _sem(), _running(true) {
}

NamedPipeThread::NamedPipeThreadWrite::~NamedPipeThreadWrite() {

}

void NamedPipeThread::NamedPipeThreadWrite::operator<<(QString const &data) {
    _mutex.lock();
    _list.enqueue(data);
    _mutex.unlock();
    _sem.release();
}

void NamedPipeThread::NamedPipeThreadWrite::send(QString const &data) {
    HANDLE hPipe;
    DWORD dwWritten;

    LPCWSTR fileToWrite = (LPCWSTR)_fileOut.utf16();
    hPipe = CreateFile(fileToWrite,
                       GENERIC_READ | GENERIC_WRITE,
                       0,
                       NULL,
                       OPEN_EXISTING,
                       0,
                       NULL);
    if (hPipe != INVALID_HANDLE_VALUE)
    {
        WriteFile(hPipe,
                  data.toStdString().c_str(),
                  data.length(),   // = length of string + terminating '\0' !!!
                  &dwWritten,
                  NULL);

        CloseHandle(hPipe);
    }
}


void NamedPipeThread::NamedPipeThreadWrite::run() {
    while (_running) {
        _sem.acquire();
        if (!_running)
            break;

        _mutex.lock();
        QString data = _list.dequeue();
        _mutex.unlock();

        send(data);

        emit sendData();

    }
}
