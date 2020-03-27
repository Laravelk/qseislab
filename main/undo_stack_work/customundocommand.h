#pragma once

#include <QUndoCommand>

class CustomUndoCommand : public QUndoCommand {
public:
  enum Type {
    EventOperation,
    AddSeismObject,
    RemoveSeismObject
  }; // TODO: implement RemoveSeismObject-Type

  explicit CustomUndoCommand(Type type) : _type(type) {}

  Type getType() const { return _type; }

private:
  const Type _type;
};
