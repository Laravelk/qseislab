#pragma once

#include "view/view.h"

#include <QObject>

#include <memory>

namespace Data {
class SeismProject;
class SeismReceiver;
} // namespace Data

namespace ReceiverOperation {
class Controller : public QObject {
  Q_OBJECT

public:
  explicit Controller(QObject *parent = nullptr);

  void viewReceivers(const std::unique_ptr<Data::SeismProject> &);
  void finish(int);

signals:
  void sendReceiver(std::unique_ptr<Data::SeismReceiver> &) const;
  void sendRemovedReceiver(const QUuid &) const;
  void finished() const;

private:
  std::unique_ptr<View> _view;

  std::vector<std::unique_ptr<Data::SeismReceiver>> _newReceivers;
  std::vector<QUuid> _removedReceivers;
};

} // namespace ReceiverOperation
