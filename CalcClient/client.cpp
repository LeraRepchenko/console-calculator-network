#include "client.h"
#include <QCoreApplication>
#include <QStringConverter>

Client::Client(const QString &host, quint16 port, QObject *parent)
    : QObject(parent)
    , m_socket(new QTcpSocket(this))
    , m_out(stdout)
    , m_inputThread(new InputThread(this))
{
    m_out.setEncoding(QStringConverter::Utf8);

    connect(m_socket, &QTcpSocket::connected, this, &Client::onConnected);
    connect(m_socket, &QTcpSocket::readyRead, this, &Client::onReadyRead);
    connect(m_socket, &QTcpSocket::disconnected, this, &Client::onDisconnected);

    connect(m_inputThread, &InputThread::lineEntered,
            this, &Client::onLineEntered);

    m_socket->connectToHost(host, port);
}

void Client::onConnected()
{
    m_out << "Подключено к серверу.\n";
    m_out << "Введите команду (add/sub/mul/div a b) или quit:\n> ";
    m_out.flush();

    // Запускаем поток чтения stdin
    m_inputThread->start();
}

void Client::onLineEntered(const QString &line)
{
    m_socket->write((line + "\n").toUtf8());
    m_out << "> ";
    m_out.flush();
}

void Client::onReadyRead()
{
    QByteArray data = m_socket->readAll();
    m_out << QString::fromUtf8(data);
    m_out.flush();
}

void Client::onDisconnected()
{
    m_out << "Соединение закрыто сервером.\n";
    m_out.flush();
    m_inputThread->quit();
    QCoreApplication::quit();
}