#pragma once

#include <QDialog>
#include <QLabel>

#include <memory>

namespace Data {
class SeismProject;
}

namespace Main {
class InfoProject : public QDialog {
public:
  InfoProject(Data::SeismProject const *const, QWidget *parent = nullptr);
};

} // namespace Main
