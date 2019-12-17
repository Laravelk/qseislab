#pragma once

#include "data/seismproject.h"

#include "undo_stack_work/customundocommand.h"

#include <memory>
#include <set>

template <typename T>
class AddObjectsToProjectCommand : public CustomUndoCommand {
public:
  explicit AddObjectsToProjectCommand(
      const std::shared_ptr<Data::SeismProject> &project,
      const std::map<QUuid, std::shared_ptr<T>> &objects)
      : CustomUndoCommand(RemoveSeismObject), _project(project),
        _objects(objects) {}

  void redo() override {
    for (auto &uuid_object : _objects) {
      _project->add<T>(uuid_object.second);
    }
  }

  void undo() override {
    for (auto &uuid_object : _objects) {
      _project->remove<T>(uuid_object.first);
    }
  }

private:
  std::shared_ptr<Data::SeismProject> _project;
  std::map<QUuid, std::shared_ptr<T>> _objects;
};
