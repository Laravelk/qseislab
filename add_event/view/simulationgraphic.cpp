#include "simulationgraphic.h"

#include "data/seismevent.h"

#include <QVBoxLayout>


namespace AddEvent {
SimulationGraphic::SimulationGraphic(QWidget *parent)
    :QFrame(parent)
{
    setMinimumWidth(850);

    _componentsTable = initComponentsTable();


    QGridLayout* layout = new QGridLayout();
    layout->addWidget(_componentsTable, 0, 0);

    setLayout(layout);
}

void SimulationGraphic::update(const std::unique_ptr<Data::SeismEvent>& event)
{
    int i = 1;
    for(const std::unique_ptr<Data::SeismComponent>& component : event->getComponents()) {
        _componentsTable->insertRow(_componentsTable->rowCount());

        _componentsTable->setItem(_componentsTable->rowCount()-1, 0, new QTableWidgetItem(QString::number(i++)));

        _componentsTable->setItem(_componentsTable->rowCount()-1, 2, new QTableWidgetItem(QString::number(component->getTraceX()->getCdpX())));
        _componentsTable->setItem(_componentsTable->rowCount()-1, 3, new QTableWidgetItem(QString::number(component->getTraceX()->getCdpY())));

        _componentsTable->setItem(_componentsTable->rowCount()-1, 5, new QTableWidgetItem(QString::number(component->getTraceY()->getCdpX())));
        _componentsTable->setItem(_componentsTable->rowCount()-1, 6, new QTableWidgetItem(QString::number(component->getTraceY()->getCdpY())));

        _componentsTable->setItem(_componentsTable->rowCount()-1, 8, new QTableWidgetItem(QString::number(component->getTraceZ()->getCdpX())));
        _componentsTable->setItem(_componentsTable->rowCount()-1, 9, new QTableWidgetItem(QString::number(component->getTraceZ()->getCdpY())));
    }
}

void SimulationGraphic::clear()
{
    int end = _componentsTable->rowCount();
    for(int i = 0; i < end; ++i) {
        _componentsTable->removeRow(0);
    }
}

QTableWidget* SimulationGraphic::initComponentsTable()
{
    QTableWidget* table = new QTableWidget(this);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setColumnCount(10);

    // configure column settings
    table->setHorizontalHeaderItem(0, new QTableWidgetItem("Component"));
    table->setColumnWidth(1,150);
    table->setHorizontalHeaderItem(1, new QTableWidgetItem(""));
    table->setColumnWidth(1,20);
    table->setHorizontalHeaderItem(2, new QTableWidgetItem("TraceX:CDP-X"));
    table->setColumnWidth(2,100);
    table->setHorizontalHeaderItem(3, new QTableWidgetItem("TraceX:CDP-Y"));
    table->setColumnWidth(3,100);
    table->setHorizontalHeaderItem(4, new QTableWidgetItem(""));
    table->setColumnWidth(4,20);
    table->setHorizontalHeaderItem(5, new QTableWidgetItem("TraceY:CDP-X"));
    table->setColumnWidth(5,100);
    table->setHorizontalHeaderItem(6, new QTableWidgetItem("TraceY:CDP-Y"));
    table->setColumnWidth(6,100);
    table->setHorizontalHeaderItem(7, new QTableWidgetItem(""));
    table->setColumnWidth(7,20);
    table->setHorizontalHeaderItem(8, new QTableWidgetItem("TraceZ:CDP-X"));
    table->setColumnWidth(8,100);
    table->setHorizontalHeaderItem(9, new QTableWidgetItem("TraceZ:CDP-Y"));
    table->setColumnWidth(9,100);


    return table;
}


} // namespace AddEvent
