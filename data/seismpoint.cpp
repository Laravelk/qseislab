#include "seismpoint.h"


namespace Data {
SeismPoint::SeismPoint(float x, float y, float z, float val)
    :_x(x), _y(y), _z(z), _val(val)
{}

float SeismPoint::getX() const
{
    return _x;
}

float SeismPoint::getY() const
{
    return _y;
}

float SeismPoint::getZ() const
{
    return _z;
}

float SeismPoint::getVal() const
{
    return _val;
}


} // namespace Data
