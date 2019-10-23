#include "polarizationanalysiswindow.h"

#include <Qt3DCore/qentity.h>
#include <Qt3DRender/qcamera.h>
#include <Qt3DRender/qcameralens.h>
#include <QtMath>

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

//#include <iostream> // TODO: delete

#include "data/seismevent.h"
#include "data/seismtrace.h"
#include "data/seismwavepick.h"

typedef Data::SeismEvent SeismEvent;
typedef Data::SeismTrace SeismTrace;
typedef Data::SeismWavePick SeismWavePick;

namespace EventOperation {

PolarizationAnalysisWindow::PolarizationAnalysisWindow(
    const std::unique_ptr<Data::SeismEvent> &event, QDialog *parent)
    : QDialog(parent), _okButton(new QPushButton("Ok")),
      _cancelButton(new QPushButton("Cancel")), _receiverBox(new QComboBox),
      _waveTypeBox(new QComboBox), _view(new Qt3DExtras::Qt3DWindow),
      _scene(new Qt3DCore::QEntity()), _event(event.get()) {
  _container = QWidget::createWindowContainer(_view);
  setMinimumSize(900, 500);

  // camera
  Qt3DRender::QCamera *camera = _view->camera();
  camera->lens()->setPerspectiveProjection(45, 16. / 9., 0.1, 1000.);
  camera->setPosition(QVector3D(1.5f, 1.5f, 1.5f));
  camera->setViewCenter(QVector3D(0, 0, 0));

  // manipulator
  Qt3DExtras::QFirstPersonCameraController *manipulator =
      new Qt3DExtras::QFirstPersonCameraController(_scene);
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

  _receiverIndex = -1;
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
  connect(event.get(), &Data::SeismEvent::changed, [this]() { update(); });
  connect(_receiverBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
          [=](int index) {
            _receiverIndex = index - 1;
            update();
          });
  connect(_waveTypeBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
          [=](int index) {
            _waveTypeIndex = index;
            update();
          });

  drawArrows();
}

void PolarizationAnalysisWindow::drawArrows() {
  _arrows.append(drawLine({0, 0, 0}, {1, 0, 0}, Qt::red, _scene));
  _arrows.append(drawLine({0, 0, 0}, {0, 1, 0}, Qt::green, _scene));
  _arrows.append(drawLine({0, 0, 0}, {0, 0, 1}, Qt::blue, _scene));

  _arrows.append(drawLine({1, 0, 0}, {0.98, 0.01, -0.01}, Qt::red, _scene));
  _arrows.append(drawLine({1, 0, 0}, {0.98, -0.01, 0.01}, Qt::red, _scene));

  _arrows.append(drawLine({0, 1, 0}, {0.01, 0.98, -0.01}, Qt::green, _scene));
  _arrows.append(drawLine({0, 1, 0}, {-0.01, 0.98, 0.01}, Qt::green, _scene));

  _arrows.append(drawLine({0, 0, 1}, {-0.01, 0.01, 0.98}, Qt::blue, _scene));
  _arrows.append(drawLine({0, 0, 1}, {0.01, -0.01, 0.98}, Qt::blue, _scene));
}

Qt3DCore::QEntity *
PolarizationAnalysisWindow::drawLine(const QVector3D &start,
                                     const QVector3D &end, const QColor &color,
                                     Qt3DCore::QEntity *_rootEntity) {
  auto *geometry = new Qt3DRender::QGeometry(_rootEntity);

  // position vertices (start and end)
  QByteArray bufferBytes;
  bufferBytes.resize(
      3 * 2 *
      sizeof(float)); // start.x, start.y, start.end + end.x, end.y, end.z
  float *positions = reinterpret_cast<float *>(bufferBytes.data());
  *positions++ = start.x();
  *positions++ = start.y();
  *positions++ = start.z();
  *positions++ = end.x();
  *positions++ = end.y();
  *positions++ = end.z();

  auto *buf = new Qt3DRender::QBuffer(geometry);
  buf->setData(bufferBytes);

  auto *positionAttribute = new Qt3DRender::QAttribute(geometry);
  positionAttribute->setName(
      Qt3DRender::QAttribute::defaultPositionAttributeName());
  positionAttribute->setVertexBaseType(Qt3DRender::QAttribute::Float);
  positionAttribute->setVertexSize(3);
  positionAttribute->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
  positionAttribute->setBuffer(buf);
  positionAttribute->setByteStride(3 * sizeof(float));
  positionAttribute->setCount(2);
  geometry->addAttribute(
      positionAttribute); // We add the vertices in the geometry

  // connectivity between vertices
  QByteArray indexBytes;
  indexBytes.resize(2 * sizeof(unsigned int)); // start to end
  unsigned int *indices = reinterpret_cast<unsigned int *>(indexBytes.data());
  *indices++ = 0;
  *indices++ = 1;

  auto *indexBuffer = new Qt3DRender::QBuffer(geometry);
  indexBuffer->setData(indexBytes);

  auto *indexAttribute = new Qt3DRender::QAttribute(geometry);
  indexAttribute->setVertexBaseType(Qt3DRender::QAttribute::UnsignedInt);
  indexAttribute->setAttributeType(Qt3DRender::QAttribute::IndexAttribute);
  indexAttribute->setBuffer(indexBuffer);
  indexAttribute->setCount(2);
  geometry->addAttribute(
      indexAttribute); // We add the indices linking the points in the geometry

  // mesh
  auto *line = new Qt3DRender::QGeometryRenderer(_rootEntity);
  line->setGeometry(geometry);
  line->setPrimitiveType(Qt3DRender::QGeometryRenderer::Lines);
  auto *material = new Qt3DExtras::QPhongMaterial(_rootEntity);
  material->setAmbient(color);

  // entity
  auto *lineEntity = new Qt3DCore::QEntity(_rootEntity);
  lineEntity->addComponent(line);
  lineEntity->addComponent(material);
  return lineEntity;
}

void PolarizationAnalysisWindow::drawCurve(
    const std::unique_ptr<Data::SeismTrace> &traceX,
    const std::unique_ptr<Data::SeismTrace> &traceY,
    const std::unique_ptr<Data::SeismTrace> &traceZ, const QColor &color,
    Qt3DCore::QEntity *_rootEntity, const int firstPointIndex,
    const int lastPointIndex, const float maxValue) {
  auto *geometry = new Qt3DRender::QGeometry(_rootEntity);
  int pointsCount = lastPointIndex - firstPointIndex;

  // position vertices (start and end)
  QByteArray bufferBytes;
  bufferBytes.resize(
      3 * pointsCount *
      sizeof(
          float)); // start.x, start.y, start.end + end.x, end.y, end.z / count
  float *positions = reinterpret_cast<float *>(bufferBytes.data());

  const std::unique_ptr<float[]> &bufferX = traceX->getBuffer();
  const std::unique_ptr<float[]> &bufferY = traceY->getBuffer();
  const std::unique_ptr<float[]> &bufferZ = traceZ->getBuffer();

  for (unsigned long long i = firstPointIndex; i < lastPointIndex; i++) {
    *positions++ = bufferX[i] / maxValue;
    *positions++ = bufferY[i] / maxValue;
    *positions++ = bufferZ[i] / maxValue;
  }

  auto *buf = new Qt3DRender::QBuffer(geometry);
  buf->setData(bufferBytes);

  auto *positionAttribute = new Qt3DRender::QAttribute(geometry);
  positionAttribute->setName(
      Qt3DRender::QAttribute::defaultPositionAttributeName());
  positionAttribute->setVertexBaseType(Qt3DRender::QAttribute::Float);
  positionAttribute->setVertexSize(3);
  positionAttribute->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
  positionAttribute->setBuffer(buf);
  positionAttribute->setByteStride(3 * sizeof(float));
  positionAttribute->setCount(pointsCount);
  geometry->addAttribute(
      positionAttribute); // We add the vertices in the geometry

  // connectivity between vertices
  QByteArray indexBytes;
  indexBytes.resize(pointsCount * sizeof(unsigned int)); // start to end
  unsigned int *indices = reinterpret_cast<unsigned int *>(indexBytes.data());

  for (int i = 0; i < pointsCount; i++) {
    *indices++ = i;
  }

  auto *indexBuffer = new Qt3DRender::QBuffer(geometry);
  indexBuffer->setData(indexBytes);

  auto *indexAttribute = new Qt3DRender::QAttribute(geometry);
  indexAttribute->setVertexBaseType(Qt3DRender::QAttribute::UnsignedInt);
  indexAttribute->setAttributeType(Qt3DRender::QAttribute::IndexAttribute);
  indexAttribute->setBuffer(indexBuffer);
  indexAttribute->setCount(pointsCount);
  geometry->addAttribute(
      indexAttribute); // We add the indices linking the points in the geometry

  // mesh
  auto *line = new Qt3DRender::QGeometryRenderer(_rootEntity);
  line->setGeometry(geometry);
  line->setPrimitiveType(Qt3DRender::QGeometryRenderer::LineStrip);
  auto *material = new Qt3DExtras::QPhongMaterial(_rootEntity);
  material->setAmbient(color);

  // entity
  auto *lineEntity = new Qt3DCore::QEntity(_rootEntity);
  lineEntity->addComponent(line);
  lineEntity->addComponent(material);
  _curves.append(lineEntity);
}

void PolarizationAnalysisWindow::drawTraces(
    const std::unique_ptr<Data::SeismComponent> &component) {
  int firstElement = 0;
  int lastElement = lastElementNumber(component);
  float maxValue = component->getMaxValue();
  Data::SeismWavePick pick;
  switch (_waveTypeIndex) {
  case SeismWavePick::PWAVE:
    pick = component->getWavePick(SeismWavePick::PWAVE);
    break;
  case Data::SeismWavePick::SWAVE:
    pick = component->getWavePick(SeismWavePick::SWAVE);
  }

  firstElement = static_cast<int>(pick.getPolarizationLeftBorder() /
                                  component->getSampleInterval());
  lastElement = static_cast<int>(pick.getPolarizationRightBorder() /
                                 component->getSampleInterval());

  drawCurve(component->getTraces().at(0), component->getTraces().at(1),
            component->getTraces().at(2), Qt::black, _scene, firstElement,
            lastElement, maxValue);
}

int PolarizationAnalysisWindow::lastElementNumber(
    const std::unique_ptr<Data::SeismComponent> &component) {
  int lastNumber = 0;
  for (auto &trace : component->getTraces()) {
    if (trace->getBufferSize() > lastNumber) {
      lastNumber = trace->getBufferSize();
    }
  }
  return lastNumber;
}

void PolarizationAnalysisWindow::update() {
  clearScene();
  if (_receiverIndex != -1 && _waveTypeIndex != 0) {
    int index = 0;
    for (auto &component : _event->getComponents()) {
      if (index == _receiverIndex) {
        drawTraces(component);
      }
      index++;
    }
  }
}

void PolarizationAnalysisWindow::clearScene() {
  //  std::cerr << " curve contain a " << _curves.size() << " elem" <<
  //  std::endl;
  for (auto &curve : _curves) {
    for (auto &component : curve->components()) {
      curve->removeComponent(component);
      delete (component);
      component = nullptr;
    }
    _curves.removeOne(curve);
    delete (curve);
    curve = nullptr;
  }
};

} // namespace EventOperation
