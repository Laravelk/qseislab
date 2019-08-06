#pragma once

#include "view.h"

#include <QObject>

#include <memory>


namespace Data {
    class SeismProject;
}

namespace ProjectOperation {
namespace OpenProject {
class Model;
class Controller : public QObject {
    Q_OBJECT

public:
    explicit Controller(QObject* parent = nullptr);

signals:
    void sendProject(std::unique_ptr<Data::SeismProject>& ) const;
    void finished() const;

private slots:
    void recvFilePath(const QString& );
    void updateProject();
    void recvNotification(const QString& );
    void finish(int);

private:
    Model* _model;

    std::unique_ptr<View> _view;
};


} // namespace OpenProject
} // namespace ProjectOperation
