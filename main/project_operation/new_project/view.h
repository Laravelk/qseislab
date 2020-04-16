#pragma once

#include <QDateTime>
#include <QDialog>
#include <QPushButton>

#include <memory>

namespace Data {
class SeismProject;
}

namespace ProjectOperation {
class InfoProject;
namespace NewProject {
class View : public QDialog {
  Q_OBJECT

public:
  explicit View(QWidget *parent = nullptr);

  void settingProjectInfo(Data::SeismProject *const);

private:
  InfoProject *_infoProject;
  QPushButton *_createButton;
  QPushButton *_cancelButton;
};

} // namespace NewProject
} // namespace ProjectOperation