#pragma once

#include <QFrame>
#include <QTableWidget>

#include <memory>

class TableAssistant : public QFrame {
  Q_OBJECT
public:
  enum Mode { ForEvents };

  TableAssistant(Mode, QWidget *parent = nullptr);

  template <typename T> void add(const std::unique_ptr<T> &);
  template <typename T> bool remove(const QUuid &);

  template <typename T>
  void setAll(const std::map<QUuid, std::unique_ptr<T>> &);

signals:
  void viewClicked(const QUuid);
  void removeClicked(const QUuid);

private:
  void clearObjectTable();
  void forEvents();
  void insertRowInFilterTable(const QString &);
  void enbledFilter(int, const QString &, const QString &);
  bool applicable(const QString &, const QString &) const;

  QTableWidget *_filterTable;
  Mode _mode;

public:
  QTableWidget *_objectsTable;
};
