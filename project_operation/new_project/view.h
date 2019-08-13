#pragma once

#include <QDateTime>
#include <QDialog>
#include <QPushButton>

namespace ProjectOperation {
class InfoProject;
namespace NewProject {
class View : public QDialog {
  Q_OBJECT

public:
  explicit View(QWidget *parent = nullptr);

  QString getName() const;
  QDate getDate() const;
  QTime getTime() const;

private:
  InfoProject *_infoProject;
  QPushButton *_createButton;
  QPushButton *_cancelButton;
};

} // namespace NewProject
} // namespace ProjectOperation
