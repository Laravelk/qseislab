#pragma once

#include <QUndoCommand>

#include <Eigen/Dense>

#include <memory>

namespace Data {
class SeismEvent;
class SeismWell;
class SeismReceiver;
}

namespace EventOperation {
namespace Modefication {
class RotateDataToEBasis : public QUndoCommand {
public:
    RotateDataToEBasis(const std::unique_ptr<Data::SeismEvent>&, const std::map<QUuid, std::unique_ptr<Data::SeismWell>>&);

    void undo() override;

    void redo() override;

private:
    const std::unique_ptr<Data::SeismReceiver>& findReceiver(const std::map<QUuid, std::unique_ptr<Data::SeismWell>>& , const QUuid& );

    const std::unique_ptr<Data::SeismEvent> &_event;

    std::vector<Eigen::MatrixXf> _originalTransitionMatrixs;
};
}
}

