#include "infowidget.h"
#include <QVBoxLayout>

#include "data/seismpolarizationanalysisdata.h"

namespace EventOperation {

InfoWidget::InfoWidget(QWidget *parent) : _polarAngle(new QLabel("Polar Angle: ")), _azimut(new QLabel("Azimut: ")),
    _planarity(new QLabel("Planarity: ")), _rectilinearity(new QLabel("Rectilinearity: "))
{
    QVBoxLayout *mainLayout = new QVBoxLayout;

    mainLayout->addWidget(_polarAngle);
    mainLayout->addWidget(_azimut);
    mainLayout->addWidget(_planarity);
    mainLayout->addWidget(_rectilinearity);

    setLayout(mainLayout);
}

void InfoWidget::update(const Data::SeismPolarizationAnalysisData &data)
{
    assert(data != std::nullopt);
    const double POLAR = data.getIncidenceDegrees();
    const double AZIMUT = data.getAzimutDegrees();
    const double PLANARITY = data.getPlanarity();
    const double RECTILINEARITY = data.getRectilinearity();

    _polarAngle->setText("Polar Angle: " + QString::number(POLAR));
    _azimut->setText("Azimut: " + QString::number(AZIMUT));
    _planarity->setText("Planarity: " + QString::number(PLANARITY));
    _rectilinearity->setText("Rectilinearity: " + QString::number(RECTILINEARITY));
}

void InfoWidget::clear()
{
    _polarAngle->setText("Polar Angle: ");
    _azimut->setText("Azimut: ");
    _planarity->setText("Planarity: ");
    _rectilinearity->setText("Rectilinearity: ");
}

}
