#include <QtTest/QtTest>

#include "../main/math/event_operation_math/ffilteringmath.h"

class TestTest2 : public QObject {
  Q_OBJECT

private slots:
  void test2() { QVERIFY(false); }
  void test3() { QVERIFY(true); }
};

QTEST_MAIN(TestTest2);
#include "test2.moc"
