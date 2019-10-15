#pragma once

#include <QObject>

#include "view/view.h"

namespace EventOperation {
namespace VolumetricScene {
class Model;
class Controller : public QObject {
  Q_OBJECT;

public:
  Controller(QObject *parent = nullptr);

  void start();

protected:
private:
  Model *_model;
  std::unique_ptr<View> _view;
};
} // namespace VolumetricScene
} // namespace EventOperation
