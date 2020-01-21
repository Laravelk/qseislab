#pragma once

#include "undo_stack_work/eventoperationundocommand.h"

#include <Eigen/Dense>

#include <memory>

namespace Data {
class SeismReceiver;
}

// namespace EventOperation {
// namespace Modefication {
class RotateData : public EventOperationUndoCommand {
public:
  class Parameters {
  public:
    enum Mode { EBASIS, INDEFINITE, RECEIVERS };
    /*
     * дефолтное значение INDEFINITE-ориентации = EBASIS-ориентация
     */

    // TODO:
    //  как реализовать, куда стоит настройка поварачивать? (enum....)
    explicit Parameters(); // NOTE: default: to e-basis

    void setMode(const Mode);
    Mode getMode() const;

    void setOrientation(const Eigen::Matrix3f &);
    const Eigen::Matrix3f &getOrientation() const;

    //    bool getToEbasis() const;
    //    void setToEbasis(bool);

    //    void setReceivers(const
    //    std::list<std::shared_ptr<Data::SeismReceiver>> &); const
    //    std::list<std::shared_ptr<Data::SeismReceiver>> &getReceivers() const;

    //    void setMatrixsBasisTo(const std::list<Eigen::Matrix3f> &);
    //    const std::list<Eigen::Matrix3f> &MatrixsBasisTo() const;

  private:
    //    std::list<Eigen::Matrix3f> _matrixsBasisTo;

    Mode _mode;

    Eigen::Matrix3f _orientation;

    //    bool _toEBasis{false};

    //    std::list<std::shared_ptr<Data::SeismReceiver>>
    //        _receivers; // TODO: этот ли контейнер?
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

  const std::shared_ptr<Data::SeismReceiver> &
  findReceiver(const std::list<std::shared_ptr<Data::SeismReceiver>> &,
               const QUuid &);

  //  Data::SeismEvent *_event;

  //  std::vector<Eigen::Matrix3f> _originalTransitionMatrixs;

  //  bool _toEbasis{false};

  std::map<QUuid, std::list<Eigen::Matrix3f>> _originalTransitionMatrixs;
};
//} // namespace Modefication
//} // namespace EventOperation
