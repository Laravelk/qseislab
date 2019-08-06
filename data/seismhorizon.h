#pragma once

#include "seismpoint.h"

#include <QJsonObject>
#include <QFileInfo>

#include <vector>


namespace Data {
class SeismHorizon {
public:
    explicit SeismHorizon();
    explicit SeismHorizon(const QJsonObject&, const QFileInfo& ) noexcept(false);

    int getPointsNumber() const;

    void addPoint(const SeismPoint& );
    const SeismPoint& getPoint(int );
    const std::vector<SeismPoint>& getPoints();

    QJsonObject& writeToJson(QJsonObject&, const QFileInfo& ) const noexcept(false);

private:
    std::vector<SeismPoint> _points;
};


} // namespace Data
