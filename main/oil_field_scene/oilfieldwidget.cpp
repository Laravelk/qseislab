#include "oilfieldwidget.h"

namespace Main {

OilFieldWidget::OilFieldWidget(QWidget* parent)
    :QWidget(parent),
    _graph(new Q3DSurface), _eventBox(new QCheckBox),
    _receiverBox(new QCheckBox), _wellBox(new QCheckBox),
    _horizonBox(new QCheckBox)
{
    // Settings
    _graph->setMinimumWidth(400);
    _graph->setMinimumHeight(700);
    _oilFieldScene = new OilFieldScene(_graph);
    QWidget *container = QWidget::createWindowContainer(_graph);
    container->setMinimumSize(QSize(400, 400));
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    container->setFocusPolicy(Qt::StrongFocus);

    _horizonBox->setChecked(true);
    _horizonBox->setText("Horizons");
    _wellBox->setChecked(true);
    _wellBox->setText("Wells");
    _receiverBox->setChecked(true);
    _receiverBox->setText("Receivers");
    _eventBox->setChecked(true);
    _eventBox->setText("Events");
    // Settings end

    // Connections
    connect(_horizonBox, &QCheckBox::stateChanged, [this]() {
        _oilFieldScene->hideAllHorizon(!_oilFieldScene->isHorizonsHide());
    });
    connect(_receiverBox, &QCheckBox::stateChanged, [this]() {
        _oilFieldScene->hideAllReceiver(!_oilFieldScene->isReceiversHide());
    });
    connect(_wellBox, &QCheckBox::stateChanged, [this]() {
        _oilFieldScene->hideAllWell(!_oilFieldScene->isWellsHide());
    });
    connect(_eventBox, &QCheckBox::stateChanged, [this]() {
        _oilFieldScene->hideAllEvent(!_oilFieldScene->isEventsHide());
    });
    // Connections end


    // Layouts
    QHBoxLayout *oilFieldSceneLayout = new QHBoxLayout();
    oilFieldSceneLayout->addWidget(container, 1);
    QVBoxLayout *checkLayout = new QVBoxLayout();
    checkLayout->addStretch(1);
    checkLayout->addWidget(_horizonBox);
    checkLayout->addWidget(_receiverBox);
    checkLayout->addWidget(_wellBox);
    checkLayout->addWidget(_eventBox);
    checkLayout->addStretch(1);
    oilFieldSceneLayout->addLayout(checkLayout);
//    QWidget *oilFieldSceneWidget = new QWidget();
//    oilFieldSceneWidget->setLayout(oilFieldSceneLayout);

    setLayout(oilFieldSceneLayout);
    // Layouts
}

void OilFieldWidget::setProject(Data::SeismProject const * const project)
{
    _oilFieldScene->setProject(project);
}

void OilFieldWidget::addEvent(Data::SeismEvent const * const event)
{
    _oilFieldScene->addEvent(event);
}

void OilFieldWidget::addHorizon(Data::SeismHorizon const * const horizon)
{
    _oilFieldScene->addHorizon(horizon);
}

void OilFieldWidget::addReceiver(Data::SeismReceiver const * const receiver)
{
    _oilFieldScene->addReceiver(receiver);
}

void OilFieldWidget::addWell(Data::SeismWell const * const well)
{
    _oilFieldScene->addWell(well);
}

bool OilFieldWidget::removeEvent(const Uuid & uuid)
{
    _oilFieldScene->removeEvent(uuid);
}

bool OilFieldWidget::removeHorizon(const Uuid & uuid)
{
    _oilFieldScene->removeHorizon(uuid);
}

bool OilFieldWidget::removeReceiver(const Uuid & uuid)
{
    _oilFieldScene->removeReceiver(uuid);
}

bool OilFieldWidget::removeWell(const Uuid & uuid)
{
    _oilFieldScene->removeWell(uuid);
}

void OilFieldWidget::hideSeismEvent(const QUuid &uuid)
{
    _oilFieldScene->hideEvent(uuid);
}

void OilFieldWidget::showSeismEvent(const QUuid &uuid)
{
    _oilFieldScene->showEvent(uuid);
}


}
