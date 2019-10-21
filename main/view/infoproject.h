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
  InfoProject(const std::unique_ptr<Data::SeismProject> &,
              QWidget *parent = nullptr);
};

} // namespace Main
