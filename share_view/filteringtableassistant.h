#pragma once

#include "data/seismevent.h"

#include <QFrame>
#include <QTableWidget>

#include <memory>
#include <set>

class CustomTableWidget;

class FilteringTableAssistant : public QFrame {
  Q_OBJECT
public:
  enum Mode { ForEvents };

  FilteringTableAssistant(Mode, QWidget *parent = nullptr);

  const std::set<QUuid> objectSelection() const;

  bool remove(const QUuid &);

  template <typename T> void add(T const *const);
  template <typename T> void update(T const *const);

  // TODO: setAll не такой должен (другая сигнатура) быть или вообще его не
  // должно быть
  template <typename T>
  void setAll(const std::map<QUuid, std::shared_ptr<T>> &);

signals:
  //  void objectSelectionChanged(const std::set<QUuid> &) const;

  void viewClicked(const QUuid &);
  void hide(const QUuid &);
  void show(const QUuid &);
  void removeClicked(const QUuid &);

  void eventsActionClicked(const std::set<QUuid> &,
                           Data::SeismEvent::TransformOperation);

  void eventsSaveClicked(const std::set<QUuid>&);

  //  // TODO: remove:
  //  void captureFocus() const;

  //  // TODO: remove:
  // protected:
  //  void focusInEvent(QFocusEvent *event) override;

private:
  std::set<QUuid> selectedUuids() const;

  void clearObjectTable();
  void forEvents();
  void insertRowInFilterTable(const QString &);
  void enbledFilter(int, const QString &, const QString &);

  template <typename mode_t>
  void applyFilterFor(int, const QString &, const QString &);
  template <typename param_t>
  void applyFilter(int, const QString &, const QString &);

  Mode _mode;
  QTableWidget *_filterTable;
  //  QTableWidget *_objectsTable;
  CustomTableWidget *_objectsTable;

  QMenu *_context;
};
