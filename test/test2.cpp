#include <QtTest/QtTest>

class TestTest2 : public QObject {
  Q_OBJECT

private slots:
  void test2() { QVERIFY(false); }
};

QTEST_MAIN(TestTest2);
#include "test2.moc"
