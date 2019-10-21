#pragma once

#include <QFrame>
#include <QPushButton>

namespace Main {
class StartPage : public QFrame {
  Q_OBJECT

public:
  explicit StartPage(QWidget *parent = nullptr);

signals:
  void newProjectClicked() const;
  void openProjectClicked() const;

private:
  QPushButton *_buttonNewProject;
  QPushButton *_buttonOpenproject;
};

} // namespace Main
