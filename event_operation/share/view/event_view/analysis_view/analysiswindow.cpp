#include "analysiswindow.h"

#include "../../tools_view/changeborderonpolarwidget.h"
#include "../../tools_view/hidewavepointswidget.h"

namespace EventOperation {

AnalysisWindow::AnalysisWindow(std::shared_ptr<Data::SeismEvent> &event, QWidget *parent) : QFrame(parent), _event(event.get()),
    _hidePointsWidget(new HideWavePointsWidget()), _changeBorderOnPolarWidget(new ChangeBorderOnPolarWidget)
{
    this->setMinimumSize(1700, 700);
    _mainLayout = new QHBoxLayout;
    _polarGraphTools = new QVBoxLayout;
    _hodogram = new PolarizationAnalysisWindow(event);
    _graph = new PolarGraph(new QPolarChart);
    _graph->update(event.get());

    connect(_hidePointsWidget, &HideWavePointsWidget::updateWaveState, [this](auto wave, auto state) {
          switch(wave) {
              case HideWavePointsWidget::Wave::PWAVE:
                  _graph->hidePWavePoints(state == HideWavePointsWidget::State::Unchecked);
                  _graph->update(_event);
                  break;
              case HideWavePointsWidget::Wave::SWAVE:
                  _graph->hideSWavePoints(state == HideWavePointsWidget::State::Unchecked);
                  _graph->update(_event);
                  break;
          }
        });

    connect(_changeBorderOnPolarWidget, &ChangeBorderOnPolarWidget::valueChanged, [this]() {
        _graph->setRadialMin(_changeBorderOnPolarWidget->getRadialMin());
        _graph->setRadialMax(_changeBorderOnPolarWidget->getRadialMax());
        _graph->setAngularMin(_changeBorderOnPolarWidget->getAngularMin());
        _graph->setAngularMax(_changeBorderOnPolarWidget->getAngularMax());
    });

    _polarGraphTools->addWidget(_hidePointsWidget);
    _polarGraphTools->addWidget(_changeBorderOnPolarWidget);
    _polarGraphTools->addStretch(1);
    _mainLayout->addLayout(_polarGraphTools);
    _mainLayout->addWidget(_graph);
    _mainLayout->addWidget(_hodogram);
    setLayout(_mainLayout);
}

void AnalysisWindow::updatePolarGraph(const Data::SeismEvent * const event)
{
    _graph->update(event);
}

void AnalysisWindow::loadEvent(std::shared_ptr<Data::SeismEvent> &event)
{
    _event = event.get();
    _hodogram->loadEvent(event);
    _graph->loadEvent(event.get());
}

void AnalysisWindow::setHodogramToDefault()
{
    _hodogram->setDefault();
}

void AnalysisWindow::removePick(int numberOfReceiver, Data::SeismWavePick::Type type)
{
    _hodogram->removePick(numberOfReceiver, type);
}

}
