#include "polarizationanalysiswindow.h"

#include <Qt3DCore/qentity.h>
#include <Qt3DRender/qcamera.h>
#include <Qt3DRender/qcameralens.h>

#include <QtGui/QScreen>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QCommandLinkButton>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QWidget>

#include <Qt3DInput/QInputAspect>

#include <Qt3DExtras/qtorusmesh.h>
#include <Qt3DRender/qeffect.h>
#include <Qt3DRender/qmaterial.h>
#include <Qt3DRender/qmesh.h>
#include <Qt3DRender/qpointlight.h>
#include <Qt3DRender/qrenderpass.h>
#include <Qt3DRender/qsceneloader.h>
#include <Qt3DRender/qtechnique.h>
#include <Qt3DRender/qtexture.h>

#include <Qt3DCore/qaspectengine.h>
#include <Qt3DCore/qtransform.h>

#include <Qt3DExtras/qforwardrenderer.h>
#include <Qt3DRender/qrenderaspect.h>

#include <Qt3DExtras/qfirstpersoncameracontroller.h>
#include <Qt3DExtras/qt3dwindow.h>

#include <iostream> // TODO: delete

#include "data/seismevent.h"

namespace EventOperation {

PolarizationAnalysisWindow::PolarizationAnalysisWindow(
    const std::unique_ptr<Data::SeismEvent> &event, QDialog *parent)
    : QDialog(parent), _okButton(new QPushButton),
      _cancelButton(new QPushButton), _receiverBox(new QComboBox),
      _waveTypeBox(new QComboBox), _view(new Qt3DExtras::Qt3DWindow),
      _scene(new Qt3DCore::QEntity()) {
  _container = QWidget::createWindowContainer(_view);
  setMinimumSize(900, 500);

  // camera
  Qt3DRender::QCamera *camera = _view->camera();
  camera->lens()->setPerspectiveProjection(45, 16. / 9., 0.1, 1000.);
  camera->setPosition(QVector3D(0.1f, 0.1f, 0.1f));
  camera->setViewCenter(QVector3D(0, 0, 0));

  // manipulator
  Qt3DExtras::QOrbitCameraController *manipulator =
      new Qt3DExtras::QOrbitCameraController(_scene);
  manipulator->setLinearSpeed(50);
  manipulator->setLookSpeed(180);
  manipulator->setCamera(camera);

  _view->setRootEntity(_scene);
  _receiverBox = new QComboBox();

  QHBoxLayout *hLayout = new QHBoxLayout(this);
  QVBoxLayout *vLayout = new QVBoxLayout();
  vLayout->setAlignment(Qt::AlignTop);
  hLayout->addWidget(_container, 1);
  hLayout->addLayout(vLayout);

  vLayout->addWidget(_cancelButton);
  vLayout->addWidget(_okButton);
  vLayout->addWidget(_receiverBox);
  vLayout->addWidget(_waveTypeBox);

  QList<QString> waveTypeList;
  QList<QString> receiverList;

  _receiverIndex = 0;
  _waveTypeIndex = 0;

  waveTypeList.append("Type Wave...");
  waveTypeList.append("PWAVE");
  waveTypeList.append("SWAVE");
  _waveTypeBox->addItems(waveTypeList);

  receiverList.append("Receiver...");
  for (int i = 0; i < event->getComponentNumber(); ++i) {
    receiverList.append(QString::number(i));
  }
  _receiverBox->addItems(receiverList);

  connect(_okButton, &QPushButton::clicked, this,
          &PolarizationAnalysisWindow::close);
  connect(_cancelButton, &QPushButton::clicked, this,
          &PolarizationAnalysisWindow::accept);
}

} // namespace EventOperation
