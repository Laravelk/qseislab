#pragma once

#include "data/seismproject.h"

#include <QFileInfo>
#include <QObject>

#include <memory>


namespace ProjectOperation {
namespace SaveProject {
class Controller : public QObject {
    Q_OBJECT

public:
    explicit Controller(QObject* parent = nullptr);

    void saveProject(std::unique_ptr<Data::SeismProject> );
    void saveAsProject(std::unique_ptr<Data::SeismProject> );

    std::unique_ptr<Data::SeismProject> getProject();

signals:
    void finished(bool) const;

private slots:
    void recvFilePath(const QString& );
    void finish(int);

private:
    bool save(const QFileInfo& );
    static void setNotification(const QString& );

    std::unique_ptr<Data::SeismProject> _project;
};


} // namespace SaveProject
} // namespace ProjectOperation
