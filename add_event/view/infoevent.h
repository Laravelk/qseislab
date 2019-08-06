#pragma once

#include <QLabel>
#include <QLineEdit>
#include <QFrame>

#include <memory>


namespace Data {
    class SeismEvent;
}

namespace AddEvent {
class InfoEvent : public QFrame {
    Q_OBJECT

public:
    explicit InfoEvent(QWidget* parent = nullptr);

    void update(const std::unique_ptr<Data::SeismEvent>& );
    void clear();

private:
    QLineEdit* _dateLineEdit;
    QLineEdit* _timeLineEdit;
    QLabel* _traceNumberLabel;
    QLabel* _lengthLabel;
    QLabel* _groupeCoordinate;
};

} // namespace AddEvent
