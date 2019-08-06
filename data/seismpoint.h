#pragma once


namespace Data {
class SeismPoint {
public:
    explicit SeismPoint(float, float, float, float);

    float getX() const;

    float getY() const;

    float getZ() const;

    float getVal() const;

private:
    float _x;
    float _y;
    float _z;
    float _val;
};


} // namespace Data
