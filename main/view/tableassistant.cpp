#include "tableassistant.h"

#include "data/seismevent.h"

#include <QBoxLayout>
#include <QCheckBox>
#include <QHeaderView>
#include <QLineEdit>
#include <QPushButton>
#include <QSplitter>
#include <QUuid>

//#include <iostream> // TODO: remove

typedef Data::SeismEvent SeismEvent;

TableAssistant::TableAssistant(Mode mode, QWidget *parent)
    : QFrame(parent), _filterTable(new QTableWidget()),
      _objectsTable(new QTableWidget()) {
  // Settings
  switch (mode) {
  case ForEvents:
    forEvents();
    break;
  }

  _objectsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
  _objectsTable->setSortingEnabled(true);
  _objectsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
  _objectsTable->setSelectionMode(QAbstractItemView::SingleSelection);

  auto horizontalHeaderObjectTable = _objectsTable->horizontalHeader();
  horizontalHeaderObjectTable->setSectionResizeMode(
      2, QHeaderView::Fixed); // fixed remove-button section
  horizontalHeaderObjectTable->setStretchLastSection(true);
  horizontalHeaderObjectTable->setDefaultAlignment(Qt::AlignLeft);
  _objectsTable->verticalHeader()->setVisible(false);

  _filterTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
  _filterTable->setSelectionBehavior(QAbstractItemView::SelectRows);
  _filterTable->setSelectionMode(QAbstractItemView::SingleSelection);

  _filterTable->setColumnCount(3);
  _filterTable->setHorizontalHeaderLabels(QStringList() << ""
                                                        << "Column"
                                                        << "Pattern");
  auto horizontalHeaderFilterTable = _filterTable->horizontalHeader();
  horizontalHeaderFilterTable->setSectionResizeMode(1, QHeaderView::Fixed);
  horizontalHeaderFilterTable->setStretchLastSection(true);
  horizontalHeaderFilterTable->setDefaultAlignment(Qt::AlignLeft);
  _filterTable->verticalHeader()->setVisible(false);

  for (int i = 3; i < _objectsTable->columnCount(); ++i) {
    insertRowInFilterTable(_objectsTable->horizontalHeaderItem(i)->text());
  }
  _filterTable->resizeColumnsToContents();
  // Settings end

  // Conections
  // Conections end

  // Layouts
  QSplitter *splitter = new QSplitter(Qt::Vertical);
  splitter->setChildrenCollapsible(false);
  splitter->addWidget(_filterTable);
  splitter->addWidget(_objectsTable);
  QVBoxLayout *mainLayout = new QVBoxLayout();
  mainLayout->addWidget(splitter);

  setLayout(mainLayout);
  // Layouts end
}

void TableAssistant::clearObjectTable() {
  const int end = _objectsTable->rowCount();
  for (int i = 0; i < end; ++i) {
    _objectsTable->removeRow(0);
  }
}

void TableAssistant::forEvents() {
  _objectsTable->setColumnCount(10);
  _objectsTable->setColumnHidden(0, true);
  _objectsTable->setColumnHidden(1, true);

  // Configure column settings
  _objectsTable->setHorizontalHeaderLabels(QStringList() << "uuid"
                                                         << "filter_names"
                                                         << ""
                                                         << "Type"
                                                         << "Component Number"
                                                         << "x"
                                                         << "y"
                                                         << "z"
                                                         << "Date"
                                                         << "Time");

  _objectsTable->resizeColumnsToContents();
}

void TableAssistant::insertRowInFilterTable(const QString &field) {
  _filterTable->insertRow(_filterTable->rowCount());
  int row = _filterTable->rowCount() - 1;

  QCheckBox *checkBox = new QCheckBox();
  QLineEdit *lineEdit = new QLineEdit();
  lineEdit->setFrame(false);

  // CheckBox setting
  QWidget *buttonWidget = new QWidget();
  QHBoxLayout *buttonLayout = new QHBoxLayout(buttonWidget);
  buttonLayout->setAlignment(Qt::AlignCenter);
  buttonLayout->addWidget(checkBox);
  buttonLayout->setContentsMargins(0, 0, 0, 0);
  _filterTable->setCellWidget(row, 0, buttonWidget);
  connect(checkBox, &QCheckBox::stateChanged,
          [this, lineEdit, field](int state) {
            enbledFilter(state, field, lineEdit->text());
          });

  // Text setting
  _filterTable->setItem(row, 1, new QTableWidgetItem(field));

  // LineEdit setting
  connect(lineEdit, &QLineEdit::textChanged, [checkBox](auto &) {
    if (Qt::Unchecked != checkBox->checkState()) {
      checkBox->setCheckState(Qt::CheckState::Unchecked);
    }
  });
  connect(lineEdit, &QLineEdit::returnPressed, [checkBox, lineEdit]() {
    if (!lineEdit->text().isEmpty()) {
      checkBox->setCheckState(Qt::CheckState::Checked);
    }
  });

  _filterTable->setCellWidget(row, 2, lineEdit);
}

void TableAssistant::enbledFilter(int enable, const QString &filterName,
                                  const QString &pattern) {
  for (int column = 0; column < _objectsTable->columnCount(); ++column) {
    auto qstr = _objectsTable->horizontalHeaderItem(column)->text();
    if (filterName == qstr) {
      for (int row = 0; row < _objectsTable->rowCount(); ++row) {
        auto item = _objectsTable->item(row, column);
        if (item) {
          QStringList filterNames =
              _objectsTable->item(row, 1)->text().split(';');
          if (Qt::Checked == enable) {
            if (applicable(pattern, item->text())) {
              if (1 == filterNames.count()) {
                _objectsTable->setRowHidden(row, true);
              }
              filterNames.append(filterName);
            }
          } else if (Qt::Unchecked == enable) {
            filterNames.removeAll(filterName);
            if (1 == filterNames.count()) {
              _objectsTable->setRowHidden(row, false);
            }
          }
          _objectsTable->item(row, 1)->setText(filterNames.join(';'));
        }
      }
      return;
    }
  }
}

bool TableAssistant::applicable(const QString &pattern,
                                const QString &value) const {

  // TODO: realize
  if (pattern.toInt() > value.toInt()) {
    return true;
  }

  return false;
}

template <>
void TableAssistant::add<SeismEvent>(const std::unique_ptr<SeismEvent> &event) {
  _objectsTable->insertRow(_objectsTable->rowCount());

  int row = _objectsTable->rowCount() - 1;

  auto &uuid = event->getUuid();
  _objectsTable->setItem(row, 0, new QTableWidgetItem(uuid.toString()));

  _objectsTable->setItem(row, 1, new QTableWidgetItem());

  QWidget *buttonWidget = new QWidget();
  QHBoxLayout *buttonLayout = new QHBoxLayout(buttonWidget);
  buttonLayout->setAlignment(Qt::AlignCenter);
  QPushButton *removeButton = new QPushButton("Remove");
  buttonLayout->addWidget(removeButton);
  buttonLayout->setContentsMargins(0, 0, 0, 0);
  _objectsTable->setCellWidget(row, 2, buttonWidget);
  connect(removeButton, &QPushButton::clicked,
          [this, uuid]() { emit removeClicked(uuid); });

  _objectsTable->setItem(
      row, 3, new QTableWidgetItem(QString::number(event->getType())));

  _objectsTable->setItem(
      row, 4,
      new QTableWidgetItem(QString::number(event->getComponentNumber())));

  if (event->isProcessed()) {
    auto &location = event->getLocation();
    _objectsTable->setItem(row, 5,
                           new QTableWidgetItem(QString::number(
                               static_cast<double>(std::get<0>(location)))));
    _objectsTable->setItem(row, 6,
                           new QTableWidgetItem(QString::number(
                               static_cast<double>(std::get<1>(location)))));
    _objectsTable->setItem(row, 7,
                           new QTableWidgetItem(QString::number(
                               static_cast<double>(std::get<2>(location)))));
  }

  _objectsTable->setItem(
      row, 8,
      new QTableWidgetItem(event->getDateTime().date().toString("dd.MM.yy")));

  _objectsTable->setItem(
      row, 9,
      new QTableWidgetItem(event->getDateTime().time().toString("hh:mm")));

  _objectsTable->resizeColumnsToContents();
}

template <> bool TableAssistant::remove<SeismEvent>(const QUuid &uuid) {
  const QString str_uuid = uuid.toString();
  for (int row = 0; row < _objectsTable->rowCount(); ++row) {
    if (str_uuid == _objectsTable->item(row, 0)->text()) {
      _objectsTable->removeRow(row);
      return true;
    }
  }
  return false;
}

template <>
void TableAssistant::setAll<SeismEvent>(
    const std::map<QUuid, std::unique_ptr<SeismEvent>> &event_map) {
  clearObjectTable();

  for (auto &uuid_event : event_map) {
    add<SeismEvent>(uuid_event.second);
  }
}
