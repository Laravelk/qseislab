#pragma once

#include <QDialog>
#include <QFormLayout>

#include <memory>

namespace Data {
class SeismHorizon;
}

namespace HorizonOperation {
namespace AddHorizon {
class View : public QDialog {
  Q_OBJECT
public:
  View(QWidget *parent = nullptr);

  // public slots:
  //  void addHorizon(const std::unique_ptr<Data::SeismHorizon> &);
  //  //    void removeHorizon(const QUuid& );

  // signals:
  //  void sendRecvFilePath(const QString &);

  // private slots:
  //  void handleDeleteHorizonClicked(const QUuid &);
  //  void handleAddHorizonClicked();
  //  void handleRecvFilePath(const QString &);

private:
  QFormLayout *_horizonsListLayout;

  QPushButton *_saveButton;
};

} // namespace AddHorizon
} // namespace HorizonOperation
