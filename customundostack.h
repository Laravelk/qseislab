#pragma once

#include <QUndoStack>

class CustomUndoStack : public QUndoStack {
  Q_OBJECT
public:
  explicit CustomUndoStack(QObject *parent = nullptr);
};
