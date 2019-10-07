#pragma once

#include <QDialog>

#include <memory>

namespace Data {
class SeismEvent;
}

namespace EventOperation {
class InfoEvent;
class Controller;
namespace ViewEvent {
class View : public QDialog {
  Q_OBJECT

public:
  explicit View(QWidget *parent = nullptr);

  void update(const std::unique_ptr<Data::SeismEvent> &);

private:
  InfoEvent *_infoEvent;
  Controller *_graphicEvent; // share_view controller
  QPushButton *_okButton;
};

} // namespace ViewEvent
} // namespace EventOperation
