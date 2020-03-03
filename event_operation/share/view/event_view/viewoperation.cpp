#include "viewoperation.h"

namespace EventOperation {

ViewOperation::ViewOperation(ViewOperation::OPERATION_TYPE type)
{
    _operationType = type;
}


void ViewOperation::setDx(int value)
{
    _dx = value;
}


void ViewOperation::setDy(int value)
{
    _dy = value;
}

qreal ViewOperation::heightAttitude() const
{
    return _heightAttitude;
}

void ViewOperation::setHeightAttitude(const qreal &heightAttitude)
{
    _heightAttitude = heightAttitude;
}

void ViewOperation::setScroll(int dx, int dy)
{
    _dx = dx;
    _dy = dy;
}

ViewOperation::OPERATION_TYPE ViewOperation::operationType() const
{
    return _operationType;
}

void ViewOperation::setOperationType(const OPERATION_TYPE &operationType)
{
    _operationType = operationType;
}

QRect ViewOperation::rect() const
{
    return _rect;
}

void ViewOperation::setRect(const QRect &rect)
{
    _rect = rect;
}

qreal ViewOperation::factor() const
{
    return _factor;
}

void ViewOperation::setFactor(const qreal &factor)
{
    _factor = factor;
}

int ViewOperation::dx() const
{
    return _dx;
}

int ViewOperation::dy() const
{
    return _dy;
}

}
