#pragma once

#include <QFileDialog>
#include <QFrame>
#include <QLabel>
#include <QPushButton>

namespace EventOperation {
namespace MoreEvents {
class FileManager : public QFrame {
  Q_OBJECT
public:
  explicit FileManager(QWidget *parent = nullptr);

  void clear();

signals:
  void sendFilePath(const QString &) const;

private slots:
  void recvFilePath(const QString &);

private:
  QLabel *_label;
  QLabel *_fileName;
  QPushButton *_browseButton;

  QFileDialog *_fileDialog;
};

} // namespace MoreEvents
} // namespace EventOperation
