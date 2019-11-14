#pragma once

#include <QFrame>
#include <QTableWidget>

#include <memory>

class TableAssistant : public QFrame {
  Q_OBJECT
public:
  enum Mode { ForHorizons, ForWells, ForReceivers };

  TableAssistant(Mode, QWidget *parent = nullptr);

  bool remove(const QUuid &);
  void requestRemoveAll();
  template <typename T>
  void setAll(const std::map<QUuid, std::shared_ptr<T>> &);

  template <typename T> void add(const std::shared_ptr<T> &);
  template <typename T> void update(const std::shared_ptr<T> &);

signals:
  void viewClicked(const QUuid &);
  void removeClicked(const QUuid &);

private:
  void clearTable();
  void forHorizons();
  void forWells();
  void forReceivers();

  Mode _mode;
  QTableWidget *_table;
};
