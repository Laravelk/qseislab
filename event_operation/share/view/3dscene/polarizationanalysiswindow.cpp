#include "polarizationanalysiswindow.h"

#include <Qt3DCore/qentity.h>
#include <Qt3DRender/qcamera.h>
#include <Qt3DRender/qcameralens.h>
#include <QtMath>
#include <iostream>

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
    const std::shared_ptr<Data::SeismEvent> &event, QDialog *parent)
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
  Qt3DExtras::QOrbitCameraController *manipulator =
      new Qt3DExtras::QOrbitCameraController(_scene);
  manipulator->setLinearSpeed(-5);
  manipulator->setLookSpeed(-120);
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

  _currentReceiverNumberString = DEFAULT_RECEIVER_STRING;
  _currentWaveTypeString = DEFAULT_WAVE_STRING;

  waveTypeList.append(DEFAULT_WAVE_STRING);
  waveTypeList.append(P_WAVE_STRING);
  waveTypeList.append(S_WAVE_STRING);
  _waveTypeBox->addItems(waveTypeList);

  receiverList.append(DEFAULT_RECEIVER_STRING);
  for (int i = 0; i < event->getComponentAmount(); ++i) {
    receiverList.append(QString::number(i));
  }
  _receiverBox->addItems(receiverList);

  connect(_okButton, &QPushButton::clicked, this,
          &PolarizationAnalysisWindow::close);
  connect(_cancelButton, &QPushButton::clicked, this,
          &PolarizationAnalysisWindow::accept);
  connect(event.get(), &Data::SeismEvent::dataChanged, [this]() { update(); });
  connect(_receiverBox,
          QOverload<const QString &>::of(&QComboBox::currentIndexChanged),
          [=](const QString string) {
            _currentReceiverNumberString = string;
            changeWaveBox();
            update();
          });
  connect(_waveTypeBox,
          QOverload<const QString &>::of(&QComboBox::currentIndexChanged),
          [=](const QString string) {
            _currentWaveTypeString = string;
            // changeReceiverNumberBox();
            update();
          });

  drawArrows();
  drawTextAxes();
}

void PolarizationAnalysisWindow::setDefault() {
  _currentWaveTypeString = DEFAULT_WAVE_STRING;
  _currentReceiverNumberString = DEFAULT_RECEIVER_STRING;
  _waveTypeBox->setCurrentIndex(0);
  _receiverBox->setCurrentIndex(0);
  update();
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

void PolarizationAnalysisWindow::drawTextAxes() {
  auto *X = new Qt3DCore::QEntity(_scene);
  auto *Y = new Qt3DCore::QEntity(_scene);
  auto *Z = new Qt3DCore::QEntity(_scene);
  auto *textMaterial = new Qt3DExtras::QPhongMaterial(_scene);
  auto *textMeshX = new Qt3DExtras::QExtrudedTextMesh();
  auto *textMeshY = new Qt3DExtras::QExtrudedTextMesh();
  auto *textMeshZ = new Qt3DExtras::QExtrudedTextMesh();
  auto *textTransformX = new Qt3DCore::QTransform();
  auto *textTransformY = new Qt3DCore::QTransform();
  auto *textTransformZ = new Qt3DCore::QTransform();
  QFont fontX("Times", 1, QFont::Bold);
  QFont fontY("Times", 1, QFont::Bold); // Y
  QFont fontZ("Times", 1, QFont::Bold); // Z
  textTransformX->setTranslation(QVector3D(1.0f, 0.1f, 0.0f));
  textTransformX->setScale(.08f);
  textTransformX->setRotationY(45);
  textTransformY->setTranslation(QVector3D(0.1f, 1.0f, 0.0f));
  textTransformY->setScale(.08f);
  textTransformY->setRotationY(45);
  textTransformZ->setTranslation(QVector3D(0.0f, 0.1f, 1.0f));
  textTransformZ->setScale(.08f);
  textTransformZ->setRotationY(45);
  textMeshX->setDepth(0);
  textMeshX->setFont(fontX);
  textMeshX->setText("X");
  textMeshY->setDepth(0);
  textMeshY->setFont(fontY);
  textMeshY->setText("Z"); // Y
  textMeshZ->setDepth(0);
  textMeshZ->setFont(fontZ);
  textMeshZ->setText("Y"); // Z
  textMaterial->setDiffuse(QColor(111, 150, 255));
  X->addComponent(textMaterial);
  X->addComponent(textMeshX);
  X->addComponent(textTransformX);
  Y->addComponent(textMaterial);
  Y->addComponent(textMeshY);
  Y->addComponent(textTransformY);
  Z->addComponent(textMaterial);
  Z->addComponent(textMeshZ);
  Z->addComponent(textTransformZ);
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
    Data::SeismTrace const *const traceX, Data::SeismTrace const *const traceY,
    Data::SeismTrace const *const traceZ, const QColor &color,
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

  auto bufferX = traceX->getBuffer();
  auto bufferY = traceY->getBuffer();
  auto bufferZ = traceZ->getBuffer();

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
    Data::SeismComponent const *const component) {
  int firstElement = 0;
  int lastElement = lastElementNumber(component);
  int pickIndex = 0;
  float maxValue = component->getMaxValue();
  Data::SeismWavePick::Type type;
  Data::SeismWavePick pick;
  if (_currentWaveTypeString == P_WAVE_STRING) {
     type = Data::SeismWavePick::PWAVE;
  } else {
     type = Data::SeismWavePick::SWAVE;
  }
  for (auto &type_pick : component->getWavePicks()) {
    if (type == type_pick.first) {
      pick = type_pick.second;
    }
  }

  // compute eigen line by eigen vector
   if (pick.getPolarizationAnalysisData() != std::nullopt) {
     Data::SeismPolarizationAnalysisData data = pick.getPolarizationAnalysisData().value();
     QVector3D eigenVector = data.getEigenVector();
     float cx1 = -2;
     float cy1 = -2;
     float cz1 = -2;
     float cx2 = 2;
     float cy2 = 2;
     float cz2 = 2;

     QVector3D point1(eigenVector.x() * cx1, eigenVector.z() * cy1, eigenVector.y() * cz1);
     QVector3D point2(eigenVector.x() * cx2, eigenVector.z() * cy2, eigenVector.y() * cz2);

     std::cerr << "Eigen Vector: " << eigenVector.x() << " " << eigenVector.y() << " " << eigenVector.z() << std::endl;

     _eigenVectorLine = drawLine(point1, point2, Qt::blue, _scene);
   }
 // end compute
  firstElement = static_cast<int>(pick.getPolarizationLeftBorder() /
                                  component->getInfo().getSampleInterval());
  lastElement = static_cast<int>(pick.getPolarizationRightBorder() /
                                 component->getInfo().getSampleInterval());

  drawCurve(component->getTraces().at(0), component->getTraces().at(2),
            component->getTraces().at(1), Qt::black, _scene, firstElement,
            lastElement, maxValue);
}

int PolarizationAnalysisWindow::lastElementNumber(
    Data::SeismComponent const *const component) {
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
  if (DEFAULT_RECEIVER_STRING != _currentReceiverNumberString &&
      DEFAULT_WAVE_STRING != _currentWaveTypeString) {
    int index = 0;
    for (auto &component : _event->getComponents()) {
      if (index == _currentReceiverNumberString.toInt()) {
        drawTraces(component);
      }
      index++;
    }
  }
}

void PolarizationAnalysisWindow::changeWaveBox() {
  QList<QString> waveTypeList;
  int itemCount = _waveTypeBox->count();
  _waveTypeBox->setCurrentIndex(0);
  for (int i = 0; i < itemCount; i++) {
    _waveTypeBox->removeItem(1);
  }
  if (DEFAULT_RECEIVER_STRING != _currentReceiverNumberString) {
    int index = 0;
    for (auto &component : _event->getComponents()) {
      if (index == _currentReceiverNumberString.toInt()) {
        for (auto &pick : component->getWavePicks()) {
          if (Data::SeismWavePick::PWAVE == pick.first) {
            waveTypeList.append(P_WAVE_STRING);
          }
          if (Data::SeismWavePick::SWAVE == pick.first) {
            waveTypeList.append(S_WAVE_STRING);
          }
        }
      }
      index++;
    }
  } else {
    waveTypeList.append(P_WAVE_STRING);
    waveTypeList.append(S_WAVE_STRING);
  }
  _waveTypeBox->addItems(waveTypeList);
  _waveTypeBox->setCurrentIndex(0);
  _currentWaveTypeString = DEFAULT_WAVE_STRING;
}

void PolarizationAnalysisWindow::changeReceiverNumberBox() {
  QList<QString> receiverWithThisWaveList;
  int itemCount = _receiverBox->count();
  _receiverBox->setCurrentIndex(0);
  for (int i = 0; i < itemCount; i++) {
    _receiverBox->removeItem(1);
  }

  if (_currentWaveTypeString != DEFAULT_WAVE_STRING) {
    int index = 0;
    for (auto &component : _event->getComponents()) {
      for (auto &pick : component->getWavePicks()) {
        qDebug() << "pick iteration";
        if ((_currentWaveTypeString == P_WAVE_STRING &&
             Data::SeismWavePick::PWAVE == pick.first) ||
            (_currentWaveTypeString == S_WAVE_STRING &&
             Data::SeismWavePick::SWAVE == pick.first)) {
          qDebug() << "find & added " << _currentWaveTypeString;
          receiverWithThisWaveList.append(QString::number(index));
        }
      }
      index++;
    }
  } else {
    for (int i = 0; i < _event->getComponentAmount(); i++) {
      receiverWithThisWaveList.append(QString::number(i));
    }
  }
  _receiverBox->addItems(receiverWithThisWaveList);
  _receiverBox->setCurrentIndex(0);
  _currentReceiverNumberString = DEFAULT_RECEIVER_STRING;
}

void PolarizationAnalysisWindow::clearScene() {
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
  if (_eigenVectorLine != nullptr) {
      for (auto &component : _eigenVectorLine->components()) {
          _eigenVectorLine->removeComponent(component);
      }
      delete _eigenVectorLine;
      _eigenVectorLine = nullptr;
  }
};

} // namespace EventOperation
