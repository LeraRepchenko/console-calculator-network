#include "calculator.h"

Calculator::Calculator(QObject *parent)
    : QObject(parent)
{
}

void Calculator::add(double a, double b)
{
    setResult(a + b);
}

void Calculator::subtract(double a, double b)
{
    setResult(a - b);
}

void Calculator::multiply(double a, double b)
{
    setResult(a * b);
}

void Calculator::divide(double a, double b)
{
    if (qFuzzyIsNull(b)) {
        setError("Деление на ноль невозможно");
        return;
    }
    setResult(a / b);
}

void Calculator::setResult(double value)
{
    m_result = value;
    m_hasError = false;
    m_errorMessage.clear();
    emit resultReady(m_result);
}

void Calculator::setError(const QString &message)
{
    m_hasError = true;
    m_errorMessage = message;
    emit errorOccurred(message);
}