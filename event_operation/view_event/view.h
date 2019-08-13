#pragma once

#include <QDialog>
#include <memory>

namespace Data {
class SeismEvent;
}

namespace EventOperation {
class InfoEvent;
class GraphicEvent;
namespace ViewEvent {
class View : public QDialog {
  Q_OBJECT

public:
  explicit View(QWidget *parent = nullptr);

  void update(const std::unique_ptr<Data::SeismEvent> &);

private:
  InfoEvent *_infoEvent;
  GraphicEvent *_graphicEvent;
  QPushButton *_okButton;
};

} // namespace ViewEvent
} // namespace EventOperation
