#pragma once

#include <QDialog>
#include <QLineEdit>

#include <memory>

namespace Data {
class SeismReceiver;
}

namespace ReceiverOperation {
class AddReceiverManager : public QDialog {
  Q_OBJECT

public:
  explicit AddReceiverManager(QWidget *parent = nullptr);

  void settingReceiverInfo(const std::unique_ptr<Data::SeismReceiver> &);

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

} // namespace ReceiverOperation