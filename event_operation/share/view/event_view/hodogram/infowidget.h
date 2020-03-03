#pragma once

#include <QFrame>
#include <QLabel>

namespace Data {
    class SeismWavePick;
    class SeismPolarizationAnalysisData;
}

namespace EventOperation {
class InfoWidget : public QFrame
{
    Q_OBJECT
public:
    InfoWidget(QWidget *parent = nullptr);
    void update(const Data::SeismPolarizationAnalysisData &);
    void clear();

private:
    QLabel *_polarAngle;
    QLabel *_azimut;
    QLabel *_planarity;
    QLabel *_rectilinearity;
};
}
