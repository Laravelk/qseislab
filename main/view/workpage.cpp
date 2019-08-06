#include "workpage.h"

#include "infoproject.h"
#include "data/seismevent.h"
#include "data/seismproject.h"

#include <QDateTime>
#include <QHBoxLayout>
#include <QPushButton>


typedef Data::SeismProject SeismProject;
typedef Data::SeismEvent SeismEvent;


namespace Main {
WorkPage::WorkPage(QWidget* parent)
    :QFrame(parent),
     _infoProject(new InfoProject(this)),
     _eventsTable(new QTableWidget(this))
{
    QPushButton* addEventButton = new QPushButton("AddEvent");
    connect(addEventButton, SIGNAL(clicked()), this, SLOT(handleAddEventClicked()));

    QPushButton* addHorizonButton = new QPushButton("AddHorizon");
    connect(addHorizonButton, SIGNAL(clicked()), this, SLOT(handleAddHorizonClicked()));

    QPushButton* saveProjectButton = new QPushButton("SaveProject");
    connect(saveProjectButton, SIGNAL(clicked()), this, SLOT(handleSaveProjectClicked()));

    QPushButton* closeProjectButton = new QPushButton("CloseProject");
    connect(closeProjectButton, SIGNAL(clicked()), this, SLOT(handleCloseProjectClicked()));

    initEventsTable(_eventsTable);


    QHBoxLayout* buttonsLayout = new QHBoxLayout();
    buttonsLayout->addWidget(addEventButton);
    buttonsLayout->addWidget(addHorizonButton);
    buttonsLayout->addWidget(saveProjectButton);
    buttonsLayout->addWidget(closeProjectButton);
    buttonsLayout->addStretch(1);

    QVBoxLayout* vLayout = new QVBoxLayout();
    vLayout->addWidget(_infoProject);
    vLayout->addStretch(1);
    vLayout->addLayout(buttonsLayout);
    vLayout->addWidget(_eventsTable);

    setLayout(vLayout);
}

void WorkPage::updateProject(const std::unique_ptr<Data::SeismProject>& project)
{
    _infoProject->update(project);

    // NOTE: Обновлять всю таблицу или проверять по id, если event с таким id в таблице?
    clearTable();
    insertEventsInTable(project);
}

void WorkPage::handleAddEventClicked()
{
    emit addEventClicked();
}

void WorkPage::handleAddHorizonClicked()
{
    emit addHorizonClicked();
}

void WorkPage::handleSaveProjectClicked()
{
    emit saveProjectClicked();
}

void WorkPage::handleCloseProjectClicked()
{
    emit closeProjectClicked();
}

void WorkPage::clearTable()
{
    int end = _eventsTable->rowCount();
    for(int i = 0; i < end; ++i) {
        _eventsTable->removeRow(0);
    }
}

void WorkPage::initEventsTable(QTableWidget* table)
{
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setColumnCount(5);

    // configure column settings
    table->setHorizontalHeaderItem(0, new QTableWidgetItem("id")); // for editing or removing events
    table->setColumnHidden(0,true);
    table->setHorizontalHeaderItem(1, new QTableWidgetItem("Component Number"));
    table->setColumnWidth(1,150);
    table->setHorizontalHeaderItem(2, new QTableWidgetItem("Type"));
    table->setColumnWidth(2,100);
    table->setHorizontalHeaderItem(3, new QTableWidgetItem("Date"));
    table->setColumnWidth(3,100);
    table->setHorizontalHeaderItem(4, new QTableWidgetItem("Time"));
    table->setColumnWidth(4,100);
}

void WorkPage::insertEventsInTable(const std::unique_ptr<Data::SeismProject>& project)
{
    for(const std::unique_ptr<SeismEvent>& event : project->getEvents()) {
        _eventsTable->insertRow(_eventsTable->rowCount());

        _eventsTable->setItem(_eventsTable->rowCount()-1, 1, new QTableWidgetItem(QString::number(event->getComponentNumber())));
//        _eventsTable->setItem(_eventsTable->rowCount()-1, 2, new QTableWidgetItem());
        _eventsTable->setItem(_eventsTable->rowCount()-1, 3, new QTableWidgetItem(event->getDateTime().date().toString("dd.MM.yy")));
        _eventsTable->setItem(_eventsTable->rowCount()-1, 4, new QTableWidgetItem(event->getDateTime().time().toString("hh:mm")));
    }
}


} // namespace Main
