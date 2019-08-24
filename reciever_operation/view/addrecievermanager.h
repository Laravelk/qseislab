#pragma once

#include <QDialog>
#include <QLineEdit>

#include <memory>

namespace Data {
class SeismReciever;
}

namespace RecieverOperation {
class AddRecieverManager : public QDialog {
  Q_OBJECT

public:
  explicit AddRecieverManager(QWidget *parent = nullptr);

  void settingRecieverInfo(const std::unique_ptr<Data::SeismReciever> &);

signals:
  void notify(const QString &);

private slots:
  void handleAccept();

private:
  QLineEdit *_nameLineEdit;
  QLineEdit *_channelNumLineEdit;
  QLineEdit *_X_LineEdit;
  QLineEdit *_Y_LineEdit;
  QLineEdit *_Z_LineEdit;
};

} // namespace RecieverOperation
