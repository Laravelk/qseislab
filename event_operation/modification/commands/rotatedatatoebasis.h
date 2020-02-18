#pragma once

#include "undo_stack_work/eventoperationundocommand.h"

#include <Eigen/Dense>

#include <memory>

namespace Data {
class SeismReceiver;
}

class RotateData : public EventOperationUndoCommand {
public:
  class Parameters {
  public:
    enum Mode { EBASIS, INDEFINITE, RECEIVERS };
    /*
     * дефолтное значение INDEFINITE-ориентации = EBASIS-ориентация
     */

    explicit Parameters(); // NOTE: default: to e-basis

    void setMode(const Mode);
    Mode getMode() const;

    void setOrientation(const Eigen::Matrix3f &);
    const Eigen::Matrix3f &getOrientation() const;

    void setReceivers(const std::list<std::shared_ptr<Data::SeismReceiver>> &);
    const std::list<std::shared_ptr<Data::SeismReceiver>> &getReceivers() const;

  private:
    Mode _mode;

    Eigen::Matrix3f _orientation;

    std::list<std::shared_ptr<Data::SeismReceiver>> _receivers;
  };

  explicit RotateData(const std::set<Data::SeismEvent *> &, const Parameters &);

protected:
  void redoForOne(Data::SeismEvent *) override;
  void undoForOne(Data::SeismEvent *) override;

private:
  const std::shared_ptr<Data::SeismReceiver> &
  findReceiver(const std::list<std::shared_ptr<Data::SeismReceiver>> &,
               const QUuid &);

  std::map<QUuid, std::vector<Eigen::Matrix3f>> _rotateOrientationMatrixs;

  std::map<QUuid, std::vector<Eigen::Matrix3f>> _originalTransitionMatrixs;
};
