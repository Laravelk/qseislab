#include "hidewavepointswidget.h"

#include <QVBoxLayout>
#include <QLabel>


namespace EventOperation {
HideWavePointsWidget::HideWavePointsWidget(QWidget *parent) : QFrame(parent), pWave(new QCheckBox()),
    sWave(new QCheckBox())
{

    setFrameStyle(1);

    pWave->setChecked(true);
    pWave->setText("pWave");
    sWave->setChecked(true);
    sWave->setText("sWave");

    connect(pWave, &QCheckBox::stateChanged, [this](int state) {
        (0 == state) ? emit updateWaveState(Wave::PWAVE, State::Unchecked)
                     : emit updateWaveState(Wave::PWAVE, State::Checked);
    });

    connect(sWave, &QCheckBox::stateChanged, [this](int state) {
        (0 == state) ? emit updateWaveState(Wave::SWAVE, State::Unchecked)
                     : emit updateWaveState(Wave::SWAVE, State::Checked);
    });

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(new QLabel("Show Wave"));
    mainLayout->addWidget(pWave);
    mainLayout->addWidget(sWave);

    setLayout(mainLayout);
}
}
