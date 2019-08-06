#pragma once

#include "seismevent.h"
#include "seismhorizon.h"

#include <memory>

#include <QObject>
#include <QJsonObject>
#include <QFileInfo>


namespace Data {
class SeismProject : public QObject {
    Q_OBJECT

public:
    explicit SeismProject(QObject* parent = nullptr);
    explicit SeismProject(const QJsonObject&, const QFileInfo&, QObject* parent = nullptr) noexcept(false);

    bool exist() const;
    bool isSaved() const;

    QJsonObject& writeToJson(QJsonObject& , const QFileInfo& ) noexcept(false);

    void setName(const QString& );
    const QString& getName() const;

    void setDate(const QDate& );
    void setTime(const QTime& );
    void setDateTime(const QDateTime& );
    const QDateTime& getDateTime() const;

    void setFileInfo(const QFileInfo& );
    const QFileInfo& getFileInfo();

    void addEvent(std::unique_ptr<SeismEvent>& );
    int getEventsNumber() const;
    const std::vector<std::unique_ptr<SeismEvent>>& getEvents() const;

    void addHorizon(std::unique_ptr<SeismHorizon>& );
    int getHorizonsNumber() const;
    const std::vector<std::unique_ptr<SeismHorizon>>& getHorizons() const;

signals:
    void changed() const;

private:
    bool _isSaved{false};

    QString _name;
    QDateTime _dateTime;
    QFileInfo _fileInfo;

    std::vector<std::unique_ptr<SeismEvent>> _events;
    std::vector<std::unique_ptr<SeismHorizon>> _horizons;
};


} // namespace Data
