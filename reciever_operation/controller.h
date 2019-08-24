#pragma once

#include "view/view.h"

#include <QObject>

#include <memory>

namespace Data {
class SeismProject;
class SeismReciever;
} // namespace Data

namespace RecieverOperation {
class Controller : public QObject {
  Q_OBJECT

public:
  explicit Controller(QObject *parent = nullptr);

  void viewRecievers(const std::unique_ptr<Data::SeismProject> &);

signals:
  void sendReciever(std::unique_ptr<Data::SeismReciever> &) const;
  void sendRemovedReciever(const QUuid &) const;
  void finished() const;

private slots:
  void handleRemoveRecieverClicked(const QUuid);
  void handleAddRecieverClicked();

  void finish(int);

private:
  std::unique_ptr<View> _view;

  std::vector<std::unique_ptr<Data::SeismReciever>> _newRecievers;
  std::vector<QUuid> _removedRecievers;
};

} // namespace RecieverOperation
