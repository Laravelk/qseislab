#include "rotatedatatoebasis.h"

#include "event_operation/modification/share_functions.h"

#include "data/seismevent.h"
#include "data/seismwell.h"

#include <assert.h>

#include <iostream> // TODO: remove

typedef Data::SeismEvent SeismEvent;
typedef Data::SeismWell SeismWell;
typedef Data::SeismReceiver SeismReceiver;

const std::shared_ptr<Data::SeismReceiver> &RotateData::findReceiver(
    const std::list<std::shared_ptr<Data::SeismReceiver>> &receivers,
    const QUuid &uuid) {

  for (auto &receiver : receivers) {
    if (uuid == receiver->getUuid()) {
      return receiver;
    }
  }

  throw std::exception();
}

RotateData::RotateData(const std::set<Data::SeismEvent *> &events,
                       const RotateData::Parameters &parameters)
    : EventOperationUndoCommand(events) {

  for (auto &event : events) {
    std::vector<Eigen::Matrix3f> rotateOrientationMatrixForOneEvent;
    if (RotateData::Parameters::Mode::RECEIVERS == parameters.getMode()) {
      for (auto &component : event->getComponents()) {
        auto &receiver = findReceiver(parameters.getReceivers(),
                                      component->getReceiverUuid());

        int size = receiver->getChannelAmount();

        Eigen::MatrixXf transitionMatrix(size, size);

        int row = 0;
        for (auto &channel : receiver->getChannels()) {
          auto &orientation = channel->getOrientation();
          Eigen::VectorXf b_basis_vec(size);
          b_basis_vec(0) = std::get<0>(orientation);
          b_basis_vec(1) = std::get<1>(orientation);
          b_basis_vec(2) = std::get<2>(orientation);
          transitionMatrix.row(row) = b_basis_vec;
          ++row;
        }
        rotateOrientationMatrixForOneEvent.push_back(transitionMatrix);
      }
    } else {
      for (int i = 0; i < event->getComponentAmount(); ++i) {
        rotateOrientationMatrixForOneEvent.push_back(
            parameters.getOrientation());
      }
    }
    _rotateOrientationMatrixs[event->getUuid()] =
        rotateOrientationMatrixForOneEvent;

    std::vector<Eigen::Matrix3f> originalTransitionMatrixForOneEvent;
    for (auto &component : event->getComponents()) {
      int size = 3;
      Eigen::MatrixXf transitionMatrix(size, size);

      int row = 0;
      for (auto &trace : component->getTraces()) {
        auto &orientation = trace->getOrientation();
        Eigen::VectorXf b_basis_vec(size);
        b_basis_vec(0) = std::get<0>(orientation);
        b_basis_vec(1) = std::get<1>(orientation);
        b_basis_vec(2) = std::get<2>(orientation);
        transitionMatrix.row(row) = b_basis_vec;
        ++row;
      }

      originalTransitionMatrixForOneEvent.push_back(transitionMatrix);
    }

    _originalTransitionMatrixs[event->getUuid()] =
        originalTransitionMatrixForOneEvent;
  }
}

void RotateData::redoForOne(Data::SeismEvent *event) {
  unsigned long i = 0;
  for (auto &component : event->getComponents()) {
    rotateDataWithTransitionMatrix(
        component, _originalTransitionMatrixs[event->getUuid()][i].inverse());
    rotateDataWithTransitionMatrix(
        component, _rotateOrientationMatrixs[event->getUuid()][i]);
    ++i;
  }

  event->changeTrigger();
}

void RotateData::undoForOne(Data::SeismEvent *event) {
  unsigned long i = 0;
  for (auto &component : event->getComponents()) {
      rotateDataWithTransitionMatrix(
          component, _rotateOrientationMatrixs[event->getUuid()][i].inverse());
      rotateDataWithTransitionMatrix(
          component, _originalTransitionMatrixs[event->getUuid()][i]);
    ++i;
  }

  event->changeTrigger();
}

RotateData::Parameters::Parameters() {
  setMode(RotateData::Parameters::Mode::EBASIS);
}

void RotateData::Parameters::setMode(RotateData::Parameters::Mode mode) {
  _mode = mode;

  switch (_mode) {
  case EBASIS:
      _orientation << 1, 0, 0, 0, 1, 0, 0, 0, 1;
      break;
  case INDEFINITE:
      break;
  case RECEIVERS:
    _orientation << 0, 0, 0, 0, 0, 0, 0, 0, 0;
    break;
  }
}

RotateData::Parameters::Mode RotateData::Parameters::getMode() const {
  return _mode;
}

void RotateData::Parameters::setOrientation(
    const Eigen::Matrix3f &orientation) {
  _orientation = orientation;
}

const Eigen::Matrix3f &RotateData::Parameters::getOrientation() const {
  return _orientation;
}

void RotateData::Parameters::setReceivers(
    const std::list<std::shared_ptr<Data::SeismReceiver>> &receivers) {
  _receivers = receivers;
}

const std::list<std::shared_ptr<Data::SeismReceiver>> &
RotateData::Parameters::getReceivers() const {
  return _receivers;
}
