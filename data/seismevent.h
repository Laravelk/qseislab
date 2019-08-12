#pragma once

#include "seismcomponent.h"

#include <memory>

#include <QDateTime>
#include <QDir>
#include <QJsonObject>
#include <QUuid>


namespace Data {
class SeismEvent {
public:
    typedef QUuid Uuid;

    static const QString _default_path;


    explicit SeismEvent();
    explicit SeismEvent(const QJsonObject&, const QDir&) noexcept(false);

    void addComponent(std::unique_ptr<SeismComponent> );

    int getComponentNumber() const;

    const std::unique_ptr<SeismComponent>& getComponent(int) const;
    const std::vector<std::unique_ptr<SeismComponent>>& getComponents() const;

    void setDateTime(const QDateTime& );
    const QDateTime& getDateTime() const;

    void setUuid(const Uuid& );
    const Uuid& getUuid() const;

    QJsonObject& writeToJson(QJsonObject&, const QDir&) noexcept(false);

private:
    QString _path;

    QDateTime _dateTime;

    Uuid  _uuid;

    std::vector<std::unique_ptr<SeismComponent>> _components; 
};


} // namespace Data
