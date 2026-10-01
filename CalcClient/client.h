#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QTextStream>
#include <QThread>

class InputThread : public QThread
{
    Q_OBJECT
public:
    explicit InputThread(QObject *parent = nullptr) : QThread(parent) {}

signals:
    void lineEntered(const QString &line);

protected:
    void run() override
    {
        QTextStream in(stdin);
        while (true) {
            QString line = in.readLine();
            if (line.isNull()) break;
            emit lineEntered(line);
        }
    }
};

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(const QString &host, quint16 port, QObject *parent = nullptr);

private slots:
    void onConnected();
    void onReadyRead();
    void onDisconnected();
    void onLineEntered(const QString &line);

private:
    QTcpSocket *m_socket;
    QTextStream m_out;
    InputThread *m_inputThread;
};

#endif // CLIENT_H