#pragma once

#include <QObject>

namespace EventOperation {
namespace VolumetricScene {

class Model : public QObject {
  Q_OBJECT;

public:
  Model(QObject *parent);

protected:
private:
};
} // namespace VolumetricScene
} // namespace EventOperation
