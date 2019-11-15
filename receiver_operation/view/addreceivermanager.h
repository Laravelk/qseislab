#pragma once

#include <QDialog>

#include <memory>

class QLineEdit;
class QComboBox;

namespace Data {
class SeismReceiver;
}

namespace ReceiverOperation {
class AddReceiverManager : public QDialog {
  Q_OBJECT

public:
  explicit AddReceiverManager(const std::map<QUuid, QString> &,
                              QWidget *parent = nullptr);

  const QUuid settingReceiverInfo(Data::SeismReceiver * const);

signals:
  void notify(const QString &);

private slots:
  void handleAccept();

private:
  QLineEdit *_nameLineEdit;
  QLineEdit *_X_LineEdit;
  QLineEdit *_Y_LineEdit;
  QLineEdit *_Z_LineEdit;

  QComboBox *_comboBox;
};

} // namespace ReceiverOperation
