#include "server.h"
#include <QStringList>
#include <QStringConverter>

Server::Server(quint16 port, QObject *parent)
    : QObject(parent)
    , m_server(new QTcpServer(this))
    , m_out(stdout)
    , m_calculator(new Calculator(this))
{
    m_out.setEncoding(QStringConverter::Utf8);

    connect(m_calculator, &Calculator::resultReady, this,
            [this](double value) {
                m_out << "Результат: " << value << "\n";
                m_out.flush();
            });
    connect(m_calculator, &Calculator::errorOccurred, this,
            [this](const QString &msg) {
                m_out << "Ошибка: " << msg << "\n";
                m_out.flush();
            });

    if (!m_server->listen(QHostAddress::Any, port)) {
        m_out << "Не удалось запустить сервер на порту " << port << "\n";
        m_out.flush();
        return;
    }
    m_out << "Сервер запущен на порту " << port << "\n";
    m_out.flush();

    connect(m_server, &QTcpServer::newConnection, this, &Server::onNewConnection);
}

void Server::onNewConnection()
{
    QTcpSocket *client = m_server->nextPendingConnection();
    m_out << "Подключился клиент: " << client->peerAddress().toString() << "\n";
    m_out.flush();

    m_buffers[client] = QByteArray();

    connect(client, &QTcpSocket::readyRead, this, &Server::onReadyRead);
    connect(client, &QTcpSocket::disconnected, this, &Server::onDisconnected);

    client->write("Добро пожаловать!\n");
    client->write("Команды: add a b | sub a b | mul a b | div a b | quit\n");
}

void Server::onReadyRead()
{
    QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
    if (!client) return;

    m_buffers[client].append(client->readAll());

    while (m_buffers[client].contains('\n')) {
        int idx = m_buffers[client].indexOf('\n');
        QByteArray lineBytes = m_buffers[client].left(idx);
        m_buffers[client].remove(0, idx + 1);

        QString line = QString::fromUtf8(lineBytes).trimmed();
        if (line.isEmpty()) continue;

        if (line.toLower() == "quit") {
            client->write("До свидания!\n");
            client->disconnectFromHost();
            return;
        }

        client->write((processCommand(line) + "\n").toUtf8());
    }
}

void Server::onDisconnected()
{
    QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
    if (!client) return;

    m_out << "Клиент отключился\n";
    m_out.flush();

    m_buffers.remove(client);
    client->deleteLater();
}

QString Server::processCommand(const QString &line)
{
    QStringList parts = line.split(' ', Qt::SkipEmptyParts);
    if (parts.size() != 3)
        return "ERROR: неверный формат. Используйте: <операция> <a> <b>";

    QString op = parts[0].toLower();
    bool ok1, ok2;
    double a = parts[1].toDouble(&ok1);
    double b = parts[2].toDouble(&ok2);

    if (!ok1 || !ok2)
        return "ERROR: не удалось преобразовать числа";

    if (op == "add") m_calculator->add(a, b);
    else if (op == "sub") m_calculator->subtract(a, b);
    else if (op == "mul") m_calculator->multiply(a, b);
    else if (op == "div") m_calculator->divide(a, b);
    else return "ERROR: неизвестная операция: " + op;

    if (m_calculator->hasError())
        return "ERROR: " + m_calculator->errorMessage();

    return QString("RESULT: %1").arg(m_calculator->result());
}