#include "filteringtableassistant.h"

#include "data/seismevent.h"
#include "parsing/evaluateExpr.h"

#include <QBoxLayout>
#include <QCheckBox>
#include <QHeaderView>
#include <QLineEdit>
#include <QPushButton>
#include <QSplitter>
#include <QUuid>

#include <assert.h>

#include <iostream> // TODO: remove

// NOTE: попробовтаь переделать на QSortFilterProxyModel

typedef Data::SeismEvent SeismEvent;

FilteringTableAssistant::FilteringTableAssistant(Mode mode, QWidget *parent)
    : QFrame(parent), _mode(mode), _filterTable(new QTableWidget()),
      _objectsTable(new QTableWidget()) {
  // Settings
  switch (_mode) {
  case ForEvents:
    forEvents();
    break;
  }

  connect(_objectsTable, &QTableWidget::itemSelectionChanged, [this] {
    auto selectedModel = _objectsTable->selectionModel();

    std::set<QUuid> selectedObjects;

    for (auto &modelItem : selectedModel->selectedRows()) {
      int row = modelItem.row();
      selectedObjects.insert(
          _objectsTable->item(row, 0)->data(Qt::DisplayRole).toUuid());
    }

    emit objectSelectionChanged(selectedObjects);
  });

  _objectsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
  _objectsTable->setSortingEnabled(true);
  _objectsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
  //  _objectsTable->setSelectionMode(QAbstractItemView::SingleSelection);

  // Double-clicking on object
  connect(_objectsTable, &QTableWidget::cellDoubleClicked,
          [this](int row, int) {
            emit viewClicked(
                _objectsTable->item(row, 0)->data(Qt::DisplayRole).toUuid());
          });

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

  for (int i = 2; i < _objectsTable->columnCount() - 2; ++i) {
    insertRowInFilterTable(_objectsTable->horizontalHeaderItem(i)->text());
  }
  _filterTable->resizeColumnsToContents();
  _filterTable->setHidden(true);

  QPushButton *hideButton = new QPushButton("filters");
  // Settings end

  // Conections
  connect(hideButton, &QPushButton::clicked, [this]() {
    this->_filterTable->setVisible(!this->_filterTable->isVisible());
  });
  // Conections end

  // Layouts
  QSplitter *splitter = new QSplitter(Qt::Vertical);
  splitter->setChildrenCollapsible(false);
  splitter->addWidget(_filterTable);
  splitter->addWidget(_objectsTable);
  QHBoxLayout *buttonsLayout = new QHBoxLayout();
  buttonsLayout->addWidget(hideButton);
  buttonsLayout->addStretch(1);
  QVBoxLayout *mainLayout = new QVBoxLayout();
  mainLayout->addLayout(buttonsLayout);
  mainLayout->addWidget(splitter);

  setLayout(mainLayout);
  // Layouts end
}

const std::set<QUuid> FilteringTableAssistant::objectSelection() const {
  auto selectedModel = _objectsTable->selectionModel();

  std::set<QUuid> selectedObjects;

  for (auto &modelItem : selectedModel->selectedRows()) {
    int row = modelItem.row();
    selectedObjects.insert(
        _objectsTable->item(row, 0)->data(Qt::DisplayRole).toUuid());
  }

  return selectedObjects;
}

bool FilteringTableAssistant::remove(const QUuid &uuid) {
  for (int row = 0; row < _objectsTable->rowCount(); ++row) {
    if (uuid == _objectsTable->item(row, 0)->data(Qt::DisplayRole).toUuid()) {
      _objectsTable->removeRow(row);
      return true;
    }
  }
  return false;
}

void FilteringTableAssistant::clearObjectTable() {
  const int end = _objectsTable->rowCount();
  for (int i = 0; i < end; ++i) {
    _objectsTable->removeRow(0);
  }
}

void FilteringTableAssistant::forEvents() {
  _objectsTable->setColumnCount(14);
  _objectsTable->setColumnHidden(0, true);
  _objectsTable->setColumnHidden(1, true);

  // Configure column settings
  _objectsTable->setHorizontalHeaderLabels(QStringList() << "uuid"
                                                         << "filter_names"
                                                         << "Name"
                                                         << "Type"
                                                         << "Receiver Amount"
                                                         << "P-Pick Amount"
                                                         << "S-Pick Amount"
                                                         << "x"
                                                         << "y"
                                                         << "z"
                                                         << "Stamp Date"
                                                         << "Stamp Time"
                                                         << ""
                                                         << "");

  _objectsTable->resizeColumnsToContents();

  auto horizontalHeaderObjectTable = _objectsTable->horizontalHeader();
  horizontalHeaderObjectTable->setSectionResizeMode(
      13, QHeaderView::Fixed); // fixed remove-button section
  horizontalHeaderObjectTable->setSectionResizeMode(
      12, QHeaderView::Stretch); // stretching pred-last section
  horizontalHeaderObjectTable->setDefaultAlignment(Qt::AlignLeft);
  _objectsTable->verticalHeader()->setVisible(false);
}

void FilteringTableAssistant::insertRowInFilterTable(const QString &field) {
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

template <typename param_t> param_t castToParam(const std::string &value) {
  if constexpr (std::is_same_v<QString, param_t>) {
    return QString::fromStdString(value);
  } else if constexpr (std::is_same_v<std::string, param_t>) {
    return value;
  } else if constexpr (std::is_same_v<QDate, param_t>) {
    return QDate::fromString(QString::fromStdString(value), "dd.MM.yy");
  } else if constexpr (std::is_same_v<QTime, param_t>) {
    return QTime::fromString(QString::fromStdString(value), "hh:mm:zzz");
  } else if constexpr (std::is_same_v<int, param_t>) {
    return std::stoi(value);
  } else if constexpr (std::is_same_v<float, param_t>) {
    return std::stof(value);
  } else {
    assert(false);
  }
}

template <typename param_t>
void FilteringTableAssistant::applyFilter(int column, const QString &pattern,
                                          const QString &filterName) {
  for (int row = 0; row < _objectsTable->rowCount(); ++row) {
    auto item = _objectsTable->item(row, column);
    if (item) {
      QStringList filterNames = _objectsTable->item(row, 1)->text().split(';');
      bool eval = TableFilterParsing::parseAndEvaluateExpr<param_t>(
          pattern.toStdString(), item->text().toStdString());
      if (!eval) {
        if (1 == filterNames.count()) {
          _objectsTable->setRowHidden(row, true);
          auto uuid =
              _objectsTable->item(row, 0)->data(Qt::DisplayRole).toUuid();
          emit hide(uuid);
        }
        filterNames.append(filterName);
      }
      _objectsTable->item(row, 1)->setText(filterNames.join(';'));
    }
  }
}

template <typename mode_t>
void FilteringTableAssistant::applyFilterFor(int column, const QString &pattern,
                                             const QString &filterName) {
  if constexpr (std::is_same_v<SeismEvent, mode_t>) {
    switch (column) {
    case 2:
      applyFilter<QString>(column, pattern, filterName);
      break;
    case 3:
    case 4:
    case 5:
    case 6:
      applyFilter<int>(column, pattern, filterName);
      break;
    case 7:
    case 8:
    case 9:
      applyFilter<float>(column, pattern, filterName);
      break;
    case 10:
      applyFilter<QDate>(column, pattern, filterName);
      break;
    case 11:
      applyFilter<QTime>(column, pattern, filterName);
      break;
    }
  }
}

void FilteringTableAssistant::enbledFilter(int enable,
                                           const QString &filterName,
                                           const QString &pattern) {

  for (int column = 0; column < _objectsTable->columnCount(); ++column) {
    if (filterName == _objectsTable->horizontalHeaderItem(column)->text()) {
      if (Qt::Unchecked == enable) {
        for (int row = 0; row < _objectsTable->rowCount(); ++row) {
          auto item = _objectsTable->item(row, column);
          if (item) {
            QStringList filterNames =
                _objectsTable->item(row, 1)->text().split(';');

            filterNames.removeAll(filterName);
            if (1 == filterNames.count()) {
              _objectsTable->setRowHidden(row, false);
              auto uuid =
                  _objectsTable->item(row, 0)->data(Qt::DisplayRole).toUuid();
              emit show(uuid);
            }
            _objectsTable->item(row, 1)->setText(filterNames.join(';'));
          }
        }
      } else if (Qt::Checked == enable) {
        switch (_mode) {
        case ForEvents:
          applyFilterFor<SeismEvent>(column, pattern, filterName);
        }
      }
      return;
    }
  }
}

template <>
void FilteringTableAssistant::update<SeismEvent>(
    SeismEvent const *const event) {
  _objectsTable->setSortingEnabled(false);

  auto &uuid = event->getUuid();
  //  auto info = event->getInfo();
  for (int row = 0; row < _objectsTable->rowCount(); ++row) {
    if (uuid == _objectsTable->item(row, 0)->data(Qt::DisplayRole).toUuid()) {
      _objectsTable->item(row, 2)->setData(Qt::DisplayRole, event->getName());
      _objectsTable->item(row, 3)->setData(Qt::DisplayRole, event->getType());
      _objectsTable->item(row, 4)->setData(Qt::DisplayRole,
                                           event->getComponentAmount());
      _objectsTable->item(row, 5)->setData(
          Qt::DisplayRole,
          event->getPickAmountByType(Data::SeismWavePick::Type::PWAVE));
      _objectsTable->item(row, 6)->setData(
          Qt::DisplayRole,
          event->getPickAmountByType(Data::SeismWavePick::Type::SWAVE));
      if (event->isProcessed()) {
        auto &location = event->getLocation();
        _objectsTable->item(row, 7)->setData(
            Qt::DisplayRole, static_cast<double>(std::get<0>(location)));
        _objectsTable->item(row, 8)->setData(
            Qt::DisplayRole, static_cast<double>(std::get<1>(location)));
        _objectsTable->item(row, 9)->setData(
            Qt::DisplayRole, static_cast<double>(std::get<2>(location)));
      }
      _objectsTable->item(row, 10)->setData(
          Qt::DisplayRole,
          event->getStampDateTime().date().toString("dd.MM.yyyy"));
      _objectsTable->item(row, 11)->setData(
          Qt::DisplayRole,
          event->getStampDateTime().time().toString("hh:mm:zzz"));
      break;
    }
  }

  _objectsTable->resizeColumnsToContents();
  _objectsTable->setSortingEnabled(true);
}

template <>
void FilteringTableAssistant::add<SeismEvent>(SeismEvent const *const event) {
  _objectsTable->setSortingEnabled(false);
  _objectsTable->insertRow(_objectsTable->rowCount());

  connect(event, &SeismEvent::infoChanged, this,
          &FilteringTableAssistant::update<SeismEvent>);

  int row = _objectsTable->rowCount() - 1;

  auto uuid = event->getUuid();
  _objectsTable->setItem(row, 0, new QTableWidgetItem(uuid.toString()));

  _objectsTable->setItem(row, 1, new QTableWidgetItem());

  //  auto info = event->getInfo();

  _objectsTable->setItem(row, 2, new QTableWidgetItem(event->getName()));

  _objectsTable->setItem(
      row, 3, new QTableWidgetItem(QString::number(event->getType())));

  _objectsTable->setItem(
      row, 4,
      new QTableWidgetItem(QString::number(event->getComponentAmount())));

  _objectsTable->setItem(
      row, 5,
      new QTableWidgetItem(QString::number(
          event->getPickAmountByType(Data::SeismWavePick::Type::PWAVE))));
  _objectsTable->setItem(
      row, 6,
      new QTableWidgetItem(QString::number(
          event->getPickAmountByType(Data::SeismWavePick::Type::SWAVE))));

  if (event->isProcessed()) {
    auto &location = event->getLocation();
    _objectsTable->setItem(row, 7,
                           new QTableWidgetItem(QString::number(
                               static_cast<double>(std::get<0>(location)))));
    _objectsTable->setItem(row, 8,
                           new QTableWidgetItem(QString::number(
                               static_cast<double>(std::get<1>(location)))));
    _objectsTable->setItem(row, 9,
                           new QTableWidgetItem(QString::number(
                               static_cast<double>(std::get<2>(location)))));
  }

  _objectsTable->setItem(
      row, 10,
      new QTableWidgetItem(
          event->getStampDateTime().date().toString("dd.MM.yy")));

  _objectsTable->setItem(
      row, 11,
      new QTableWidgetItem(
          event->getStampDateTime().time().toString("hh:mm:zzz")));

  QPushButton *removeButton = new QPushButton();
  QIcon icon(":/remove_button.png");
  removeButton->setStyleSheet("background-color:white; border-style: outset");
  removeButton->setIcon(icon);
  _objectsTable->setCellWidget(row, 13, removeButton);
  connect(removeButton, &QPushButton::clicked,
          [this, uuid]() { emit removeClicked(uuid); });

  _objectsTable->resizeColumnsToContents();
  _objectsTable->setSortingEnabled(true);
}

template <>
void FilteringTableAssistant::setAll<SeismEvent>(
    const std::map<QUuid, std::shared_ptr<SeismEvent>> &event_map) {
  clearObjectTable();

  for (auto &uuid_event : event_map) {
    add<SeismEvent>(uuid_event.second.get());
  }
}
