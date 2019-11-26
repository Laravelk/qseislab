#pragma once

#include <QFrame>

#include <memory>

class QTableWidget;

class TableAssistant : public QFrame {
  Q_OBJECT
public:
  enum Mode { ForHorizons, ForWells, ForReceivers };

  explicit TableAssistant(Mode, QWidget *parent = nullptr);

  bool remove(const QUuid &);

  template <typename T> void add(T const *const);
  template <typename T> void update(T const *const);

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
