#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHash>
#include <QTextStream>
#include "calculator.h"

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(quint16 port, QObject *parent = nullptr);

private slots:
    void onNewConnection();
    void onReadyRead();
    void onDisconnected();

private:
    QString processCommand(const QString &line);

    QTcpServer *m_server;
    QHash<QTcpSocket*, QByteArray> m_buffers;
    QTextStream m_out;
    Calculator *m_calculator;
};

#endif // SERVER_H