#pragma once

#include <QFrame>
#include <QTableWidget>

#include <memory>
#include <set>

class FilteringTableAssistant : public QFrame {
  Q_OBJECT
public:
  enum Mode { ForEvents };

  FilteringTableAssistant(Mode, QWidget *parent = nullptr);

  const QUuid getFocusObject() const;

  bool remove(const QUuid &);

  template <typename T> void add(const std::shared_ptr<T> &);
  template <typename T> void update(const std::shared_ptr<T> &);
  template <typename T>
  void setAll(const std::map<QUuid, std::shared_ptr<T>> &);

signals:
  void objectSelectionChanged(const std::set<QUuid> &) const;

  void viewClicked(const QUuid &);
  void hide(const QUuid &);
  void show(const QUuid &);
  void removeClicked(const QUuid &);

private:
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
  QTableWidget *_objectsTable;
};
