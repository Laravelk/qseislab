#pragma once

#include <QFrame>
#include <QTableWidget>

#include <memory>


namespace Data {
    class SeismEvent;
}

namespace AddEvent {
class SimulationGraphic : public QFrame {
    Q_OBJECT

public:
    explicit SimulationGraphic(QWidget* parent = nullptr);

    void update(const std::unique_ptr<Data::SeismEvent>& );
    void clear();

private:
    QTableWidget* initComponentsTable();

    QTableWidget* _componentsTable;
};


} // namespace AddEvent
