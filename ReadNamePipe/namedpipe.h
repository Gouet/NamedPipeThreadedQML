#ifndef NAMEDPIPE_H
#define NAMEDPIPE_H

#include <QString>
#include <QObject>
#include <QThread>
#include <QSemaphore>
#include <QQueue>
#include <QMutex>

// THREAD READ
namespace  NamedPipeThread {
class NamedPipeThreadRead : public QThread {
    Q_OBJECT
public:
    NamedPipeThreadRead(QString const &fileIn);
    ~NamedPipeThreadRead();

protected:
    void run();

signals:
    void receiveData(QString);

private:
    QString _fileIn;
};
}

// THREAD WRITE
namespace  NamedPipeThread {
class NamedPipeThreadWrite : public QThread {
    Q_OBJECT
public:
    NamedPipeThreadWrite(QString const &fileOut);
    ~NamedPipeThreadWrite();

public:
    void operator<<(QString const &data);

protected:
    void run();

private:
    void send(QString const &data);

signals:
    void sendData();

private:
    QString _fileOut;
    QSemaphore _sem;
    bool _running;
    QQueue<QString> _list;
    QMutex _mutex;
};
}

// NAMED PIPE CLASS
class NamedPipe : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString fileIn READ fileIn WRITE setFileIn NOTIFY fileInChanged)
    Q_PROPERTY(QString fileOut READ fileOut WRITE setFileOut NOTIFY fileOutChanged)

    //QString const &fileIn, QString const &fileOut
public:
    NamedPipe();
    ~NamedPipe();

public:
    QString const &fileIn() const;
    QString const &fileOut() const;

    void setFileIn(QString const &);
    void setFileOut(QString const &);

signals:
    void fileInChanged();
    void fileOutChanged();

public:
    void operator<<(QString const &);

    Q_INVOKABLE void send(QString const &);

signals:
    void receiveData(QString data);
    void sendData();

private:
    QString _fileIn;
    QString _fileOut;

    NamedPipeThread::NamedPipeThreadRead *threadRead;
    NamedPipeThread::NamedPipeThreadWrite *threadWrite;

    // Un Thread write
};

#endif // NAMEDPIPE_H
