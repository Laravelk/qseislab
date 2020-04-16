#include <QtTest/QtTest>
#include <vector>

#include <Eigen/Eigenvalues>
#include <Eigen/Jacobi>
#include <Eigen/SVD>
#include <iostream>

#include "data/vectordata.h"
#include "../main/math/event_operation_math/svdmath.h"

class SVDTest : public QObject {
  Q_OBJECT

private slots:
  void PolarAnalysisTest1() {
    const int FIRST_INDEX = 820;
    const int LAST_INDEX = 980;
    const double MAX_VALUE = 4.68327e-007;
    const float MAX_SING = 4.02498;
    const float AZIMUT = 1.07111;
    const float INCENDCE = 1.51545;
    const float PLANARITY = 0.780047;
    const float RECTILINEARITY = -2.10084;
    Eigen::MatrixXf matrix =
        fillMatrix(vectorData.vectorXTest1, vectorData.vectorYTest1,
                   vectorData.vectorZTest1, FIRST_INDEX, LAST_INDEX, MAX_VALUE);
    Data::SeismPolarizationAnalysisData data =
        EventMath::SVDMath::calculatePolarizationData(matrix);
    QVERIFY(comparePolarAnalysisParam(MAX_SING, AZIMUT, INCENDCE, PLANARITY,
                                      RECTILINEARITY, data));
  }

  void PolarAnalysisTest2() {
    const int FIRST_INDEX = 968;
    const int LAST_INDEX = 1008;
    const double MAX_VALUE = 3.32759e-007;
    const float MAX_SING = 3.29088;
    const float AZIMUT = 0.548713;
    const float INCENDCE = 1.3966;
    const float PLANARITY = 0.895591;
    const float RECTILINEARITY = -1.37513;
    Eigen::MatrixXf matrix =
        fillMatrix(vectorData.vectorXTest2, vectorData.vectorYTest2,
                   vectorData.vectorZTest2, FIRST_INDEX, LAST_INDEX, MAX_VALUE);
    Data::SeismPolarizationAnalysisData data =
        EventMath::SVDMath::calculatePolarizationData(matrix);
    QVERIFY(comparePolarAnalysisParam(MAX_SING, AZIMUT, INCENDCE, PLANARITY,
                                      RECTILINEARITY, data));
  }

  void PolarAnalysisTest3() {
    const int FIRST_INDEX = 1189;
    const int LAST_INDEX = 1208;
    const double MAX_VALUE = 3.83084e-007;
    const float MAX_SING = 4.85772;
    const float AZIMUT = -0.979415;
    const float INCENDCE = 1.52323;
    const float PLANARITY = 0.926202;
    const float RECTILINEARITY = -2.00082;
    Eigen::MatrixXf matrix =
        fillMatrix(vectorData.vectorXTest3, vectorData.vectorYTest3,
                   vectorData.vectorZTest3, FIRST_INDEX, LAST_INDEX, MAX_VALUE);
    Data::SeismPolarizationAnalysisData data =
        EventMath::SVDMath::calculatePolarizationData(matrix);
    QVERIFY(comparePolarAnalysisParam(MAX_SING, AZIMUT, INCENDCE, PLANARITY,
                                      RECTILINEARITY, data));
  }

private:
  VectorData vectorData;

  bool compareFloat(float a, float b) const { return abs(b - a) < 0.05; }

  bool comparePolarAnalysisParam(
      const float MAX_SING, const float AZIMUT, const float INCENDCE,
      const float PLANARITY, const float RECTILINEARITY,
      const Data::SeismPolarizationAnalysisData &data) const {
    return compareFloat(MAX_SING, data.getMaxSingularValue()) &&
           compareFloat(AZIMUT, data.getAzimutInRadian()) &&
           compareFloat(INCENDCE, data.getIncidenceInRadian()) &&
           compareFloat(PLANARITY, data.getPlanarity()) &&
           compareFloat(RECTILINEARITY, data.getRectilinearity());
  }

  Eigen::MatrixXf fillMatrix(std::vector<float> X, std::vector<float> Y,
                             std::vector<float> Z, const int FIRST_INDEX,
                             const int LAST_INDEX, const double MAX_VALUE) {
    Q_ASSERT(LAST_INDEX > FIRST_INDEX);
    Eigen::MatrixXf pointMatrix(3, LAST_INDEX - FIRST_INDEX);

    Eigen::RowVectorXf vectorX(LAST_INDEX - FIRST_INDEX);
    Eigen::RowVectorXf vectorY(LAST_INDEX - FIRST_INDEX);
    Eigen::RowVectorXf vectorZ(LAST_INDEX - FIRST_INDEX);

    for (int i = 0; i < LAST_INDEX - FIRST_INDEX; i++) {
      vectorX[i] = X[i] / MAX_VALUE;
      vectorY[i] = Y[i] / MAX_VALUE;
      vectorZ[i] = Z[i] / MAX_VALUE;
    }
    pointMatrix << vectorX, vectorY, vectorZ;
    return pointMatrix;
  }
};

QTEST_MAIN(SVDTest);
#include "SVDTest.moc"
