#pragma once

#include "undo_stack_work/eventoperationundocommand.h"

#include <Eigen/Dense>

#include <memory>

// namespace EventOperation {
// namespace Modefication {
class RotateData : public EventOperationUndoCommand {
public:
  class Parameters {
  public:
    // TODO:
    //  как реализовать, куда стоит настройка поварачивать? (enum....)
    explicit Parameters(); // NOTE: default: to e-basis

    void setMatrixsBasisTo(const std::vector<Eigen::Matrix3f> &);
    const std::vector<Eigen::Matrix3f> &MatrixsBasisTo() const;

  private:
    std::vector<Eigen::Matrix3f> _matrixsBasisTo;
  };

  // TODO: implement!!
  explicit RotateData(const std::set<Data::SeismEvent *> &, const Parameters &);

protected:
  void redoForOne(Data::SeismEvent *) override;
  void undoForOne(Data::SeismEvent *) override;

private:
  //  const std::shared_ptr<Data::SeismReceiver> &
  //  findReceiver(const std::map<QUuid, std::shared_ptr<Data::SeismWell>> &,
  //               const QUuid &);

  //  const std::shared_ptr<Data::SeismReceiver> &
  //  findReceiver(const std::list<std::shared_ptr<Data::SeismReceiver>> &,
  //               const QUuid &);

  Data::SeismEvent *_event;

  std::vector<Eigen::Matrix3f> _originalTransitionMatrixs;
};
//} // namespace Modefication
//} // namespace EventOperation
