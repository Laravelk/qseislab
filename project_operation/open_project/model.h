#pragma once

#include <QObject>

#include <memory>


namespace Data {
    class SeismProject;
}

namespace ProjectOperation {
namespace OpenProject {
class Model : public QObject {
    Q_OBJECT

public:
    explicit Model(QObject* );

    bool isValid() const;

    void setFilePath(const QString& );
    const QString& getFilePath() const;

    const QString& getErrMsg() const;

    std::unique_ptr<Data::SeismProject>& getSeismProject();

signals:
    void changed() const;
    void notify(const QString& ) const;

private:
    bool _isValid{false};
    QString _filePath;
    QString _errmsg;

    std::unique_ptr<Data::SeismProject> _project;
};


} // namespace OpenProject
} // namespace ProjectOperation
