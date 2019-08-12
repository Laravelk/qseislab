#pragma once

#include <QJsonObject>
#include <QDir>
#include <QUuid>

#include <tuple>
#include <vector>


namespace Data {
class SeismHorizon {
public:
    typedef QUuid Uuid;
    typedef std::tuple<float,float,float,float> SeismPoint;

    static const QString _default_path;


    explicit SeismHorizon();
    explicit SeismHorizon(const QJsonObject&, const QDir& ) noexcept(false);

    int getPointsNumber() const;

    void addPoint(const SeismPoint& );
    const SeismPoint& getPoint(int);
    const std::vector<SeismPoint>& getPoints();

    void setUuid(const Uuid& );
    const Uuid& getUuid() const;

    QJsonObject& writeToJson(QJsonObject&, const QDir& ) noexcept(false);

private:
    QString _path;

    Uuid _uuid;

    std::vector<SeismPoint> _points;
};


} // namespace Data
