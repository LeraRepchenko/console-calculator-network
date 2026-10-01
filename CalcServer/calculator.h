#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QObject>
#include <QString>

class Calculator : public QObject
{
    Q_OBJECT

public:
    explicit Calculator(QObject *parent = nullptr);

    double result() const { return m_result; }
    bool hasError() const { return m_hasError; }
    QString errorMessage() const { return m_errorMessage; }

public slots:
    void add(double a, double b);
    void subtract(double a, double b);
    void multiply(double a, double b);
    void divide(double a, double b);

signals:
    void resultReady(double result);
    void errorOccurred(const QString &message);

private:
    void setResult(double value);
    void setError(const QString &message);

    double m_result = 0.0;
    bool m_hasError = false;
    QString m_errorMessage;
};

#endif // CALCULATOR_H