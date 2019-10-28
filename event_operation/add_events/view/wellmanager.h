#pragma once

#include <QComboBox>
#include <QFrame>
#include <QUuid>

#include <memory>

namespace EventOperation {
namespace Generic {
class FileManager;
class WellManager : public QFrame {
  Q_OBJECT

public:
  explicit WellManager(const std::map<QUuid, QString> &,
                       QWidget *parent = nullptr);

  void updateWellNames(const std::map<QUuid, QString> &);

  void clear();

signals:
    void sendWellUuidAndFilePath(const QUuid&, const QString &) const;
  void removeClicked(QWidget *sender, const QUuid &) const;

private:
  QComboBox *_comboBox;
  FileManager *_fileManager;

  QUuid _wellUuid;
};

} // namespace Generic
} // namespace EventOperation
