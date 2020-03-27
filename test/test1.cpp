#include <QtTest/QtTest>

class TestTest1 : public QObject {
  Q_OBJECT

private slots:
  void test1() { QVERIFY(true); }
};

QTEST_MAIN(TestTest1);
#include "test1.moc"
