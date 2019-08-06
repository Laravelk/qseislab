#pragma once

#include "seismcomponent.h"

#include <memory>

#include <QDateTime>
#include <QFileInfo>
#include <QJsonObject>


namespace Data {
class SeismEvent {
public:
    explicit SeismEvent();
    explicit SeismEvent(const QJsonObject&, const QFileInfo&) noexcept(false);

    void addComponent(std::unique_ptr<SeismComponent>& );

    int getComponentNumber() const;

    const std::unique_ptr<SeismComponent>& getComponent(int) const;
    const std::vector<std::unique_ptr<SeismComponent>>& getComponents() const;

    void setDateTime(const QDateTime& );
    const QDateTime& getDateTime() const;

    QJsonObject& writeToJson(QJsonObject&, const QFileInfo&) const noexcept(false);

private:
    static bool readData(QString, std::unique_ptr<float[]>&, std::unique_ptr<float[]>&, std::unique_ptr<float[]>&);
    static bool writeData(QString);

    QDateTime _dateTime;

    std::vector<std::unique_ptr<SeismComponent>> _components;
};


} // namespace Data
