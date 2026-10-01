#include <QtTest>
#include <QSignalSpy>
#include "calculator.h"

class TestCalculator : public QObject
{
    Q_OBJECT

private slots:
    void testAdd()
    {
        Calculator calc;
        QSignalSpy spy(&calc, &Calculator::resultReady);
        calc.add(5, 3);
        QCOMPARE(calc.result(), 8.0);
        QCOMPARE(spy.count(), 1);
    }

    void testSubtract()
    {
        Calculator calc;
        calc.subtract(100, 37);
        QCOMPARE(calc.result(), 63.0);
    }

    void testMultiply()
    {
        Calculator calc;
        calc.multiply(4, 2.5);
        QCOMPARE(calc.result(), 10.0);
    }

    void testDivide()
    {
        Calculator calc;
        calc.divide(10, 2);
        QCOMPARE(calc.result(), 5.0);
    }

    void testDivideByZero()
    {
        Calculator calc;
        QSignalSpy spy(&calc, &Calculator::errorOccurred);
        calc.divide(10, 0);
        QVERIFY(calc.hasError());
        QCOMPARE(spy.count(), 1);
        QCOMPARE(calc.errorMessage(), QString("Деление на ноль невозможно"));
    }

    void testErrorReset()
    {
        Calculator calc;
        calc.divide(10, 0);
        QVERIFY(calc.hasError());
        calc.add(1, 1);
        QVERIFY(!calc.hasError());
    }
};

QTEST_APPLESS_MAIN(TestCalculator)
#include "test_calculator.moc"