#pragma once

#include <QUndoCommand>

#include <Eigen/Dense>

#include <memory>

namespace Data {
class SeismEvent;
class SeismWell;
class SeismReceiver;
} // namespace Data

namespace EventOperation {
namespace Modefication {
class RotateDataToEBasis : public QUndoCommand {
public:
  RotateDataToEBasis(Data::SeismEvent *,
                     const std::map<QUuid, std::shared_ptr<Data::SeismWell>> &);

  void undo() override;

  void redo() override;

private:
  const std::shared_ptr<Data::SeismReceiver> &
  findReceiver(const std::map<QUuid, std::shared_ptr<Data::SeismWell>> &,
               const QUuid &);

  Data::SeismEvent *_event;

  std::vector<Eigen::MatrixXf> _originalTransitionMatrixs;
};
} // namespace Modefication
} // namespace EventOperation
