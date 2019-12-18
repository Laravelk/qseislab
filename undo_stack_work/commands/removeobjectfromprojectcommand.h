#pragma once

#include "data/seismproject.h"

#include "undo_stack_work/customundocommand.h"

#include <memory>
#include <set>

template <typename T>
class RemoveObjectFromProjectCommand : public CustomUndoCommand {
public:
  explicit RemoveObjectFromProjectCommand(
      const std::shared_ptr<Data::SeismProject> &project, const QUuid &uuid)
      : CustomUndoCommand(RemoveSeismObject), _project(project),
        _object(_project->get<T>(uuid)) {}

  void redo() override { _project->remove<T>(_object->getUuid()); }

  void undo() override { _project->add<T>(_object); }

private:
  std::shared_ptr<Data::SeismProject> _project;
  std::shared_ptr<T> _object;
};
