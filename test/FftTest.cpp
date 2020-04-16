#include <QtTest/QtTest>
#include <vector>

#include "../main/math/event_operation_math/ffilteringmath.h"
#include "data/FFtVectorData.h"

class FftTest : public QObject {
  Q_OBJECT

private slots:
  void FftTest1() {
    const int F1 = 10;
    const int F2 = 20;
    const int F3 = 70;
    const int F4 = 1000;
    const int SAMPLE_INTERVAL = 250;
    std::vector<float> timevec = data.oldTest1;
    EventMath::FFilteringMath::FFiltering(timevec, F1, F2, F3, F4,
                                          SAMPLE_INTERVAL);
    QVERIFY(data.resultatTest1 == timevec);
  }

private:
  FFtVectorData data;
};

QTEST_MAIN(FftTest);
#include "FftTest.moc"
