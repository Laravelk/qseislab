#include "rotatedatatoebasis.h"

#include "event_operation/modification/share_functions.h"

#include "data/seismevent.h"
#include "data/seismwell.h"

#include <assert.h>

#include <iostream> // TODO: remove

typedef Data::SeismEvent SeismEvent;
typedef Data::SeismWell SeismWell;
typedef Data::SeismReceiver SeismReceiver;

// namespace EventOperation {
// namespace Modefication {
RotateData::RotateData(const QUuid &shareUuid, Data::SeismEvent *event,
                       const Parameters &parameters)
    : CustomIndividualUndoCommand(shareUuid), _event(event) {

  setText("Rotate Data to E-Basis");

  //  for (auto &matrix : parameters.getRotateMatrixs()) {
  //    _originalTransitionMatrixs.push_back(matrix.replicate(3, 3));
  //  }

  //  for (auto &component : _event->getComponents()) {
  //    auto &receiver =
  //        findReceiver(parameters.getReceivers(),
  //        component->getReceiverUuid());

  //    int size = receiver->getChannelAmount();

  //    Eigen::MatrixXf transitionMatrix(size, size);

  //    int row = 0;
  //    for (auto &channel : receiver->getChannels()) {
  //      auto &orientation = channel->getOrientation();
  //      Eigen::VectorXf b_basis_vec(size);
  //      b_basis_vec(0) = std::get<0>(orientation);
  //      b_basis_vec(1) = std::get<1>(orientation);
  //      b_basis_vec(2) = std::get<2>(orientation);
  //      transitionMatrix.row(row) = b_basis_vec;
  //      ++row;
  //    }

  //    _originalTransitionMatrixs.push_back(transitionMatrix);
  //  }
}

// RotateDataToEBasis::RotateDataToEBasis(
//    SeismEvent *event,
//    const std::map<QUuid, std::shared_ptr<SeismWell>> &wells_map)
//    : _event(event) {
//  setText("Rotate Data to E-Basis");
//  for (auto &component : _event->getComponents()) {
//    auto &receiver = findReceiver(wells_map, component->getReceiverUuid());

//    int size = receiver->getChannelAmount();

//    Eigen::MatrixXf transitionMatrix(size, size);

//    int row = 0;
//    for (auto &channel : receiver->getChannels()) {
//      auto &orientation = channel->getOrientation();
//      Eigen::VectorXf b_basis_vec(size);
//      // NOTE: bad way (only for 3)
//      b_basis_vec(0) = std::get<0>(orientation);
//      b_basis_vec(1) = std::get<1>(orientation);
//      b_basis_vec(2) = std::get<2>(orientation);
//      transitionMatrix.row(row) = b_basis_vec;
//      ++row;
//    }

//    _originalTransitionMatrixs.push_back(transitionMatrix);
//  }
//}

void RotateData::undo() {
  unsigned long i = 0;
  for (auto &component : _event->getComponents()) {
    rotateDataWithTransitionMatrix(component, _originalTransitionMatrixs[i]);
    ++i;
  }
  //  _event->removeTransformOperation(SeismEvent::RotateDataToEBasis);
}

void RotateData::redo() {
  unsigned long i = 0;
  for (auto &component : _event->getComponents()) {
    rotateDataWithTransitionMatrix(component,
                                   _originalTransitionMatrixs[i].transpose());
    ++i;
  }
  //  _event->addTransformOperation(SeismEvent::RotateDataToEBasis);
}

bool RotateData::is(SeismEvent::TransformOperation oper) const {
  return oper == Data::SeismEvent::TransformOperation::RotateData;
}

// const std::shared_ptr<SeismReceiver> &RotateDataToEBasis::findReceiver(
//    const std::map<QUuid, std::shared_ptr<SeismWell>> &wells_map,
//    const QUuid &receiver_uuid) {
//  //  for (auto &uuid_well : wells_map) {
//  //    for (auto &receiver : uuid_well.second->getReceivers()) {
//  //      if (receiver_uuid == receiver->getUuid()) {
//  //        return receiver;
//  //      }
//  //    }
//  //  }
//  assert(false); // NOTE: как обрабатывать такие случаи?
//}

// const std::shared_ptr<Data::SeismReceiver> &RotateDataToEBasis::findReceiver(
//    const std::list<std::shared_ptr<Data::SeismReceiver>> &receivers,
//    const QUuid &uuid) {

//  for (auto &receiver : receivers) {
//    if (uuid == receiver->getUuid()) {
//      return receiver;
//    }
//  }

//  assert(false);
//}
//} // namespace Modefication
//} // namespace EventOperation

RotateData::Parameters::Parameters() {
  Eigen::Matrix3f e_matrix;
  e_matrix << 1, 0, 0, 0, 1, 0, 0, 0, 1;
  for (int i = 0; i < 3; ++i) {
    _matrixsBasisTo.push_back(e_matrix);
  }
}

void RotateData::Parameters::setMatrixsBasisTo(
    const std::vector<Eigen::Matrix3f> &matrixsBasisTo) {

  _matrixsBasisTo.clear();
  for (auto &matrix : matrixsBasisTo) {
    _matrixsBasisTo.push_back(matrix.replicate(3, 3));
  }
}

const std::vector<Eigen::Matrix3f> &
RotateData::Parameters::MatrixsBasisTo() const {
  return _matrixsBasisTo;
}
