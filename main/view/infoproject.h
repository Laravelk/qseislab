#pragma once

#include <QFrame>
#include <QLabel>

#include <memory>

namespace Data {
class SeismProject;
}

namespace Main {
class InfoProject : public QFrame {
public:
  InfoProject(QWidget *parent = nullptr);

  void update(const std::unique_ptr<Data::SeismProject> &);

  void addEvent();
  void removeEvent();

  void addHorizon();
  void removeHorizon();

  void addReceiver();
  void removeReceiver();

  void addWell();
  void removeWell();

private:
  QLabel *_nameLabel;
  QLabel *_dateLabel;
  QLabel *_timeLabel;
  QLabel *_eventsNumber;
  QLabel *_horizonsNumber;
  QLabel *_receiversNumber;
  QLabel *_wellsNumber;
};

} // namespace Main
