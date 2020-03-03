#pragma once

#include <QList>
#include <QRect>

namespace EventOperation {
class ViewOperation {
public:
  enum OPERATION_TYPE { SCROLL = 1, ZOOM_IN = 2, ZOOM = 3 };
  ViewOperation(OPERATION_TYPE);
  void setScroll(int _dx, int _dy);

  OPERATION_TYPE operationType() const;
  void setOperationType(const OPERATION_TYPE &operationType);

  QRect rect() const;
  void setRect(const QRect &rect);

  qreal factor() const;
  void setFactor(const qreal &factor);

  int dx() const;
  void setDx(int dx);

  int dy() const;
  void setDy(int dy);

  qreal heightAttitude() const;
  void setHeightAttitude(const qreal &heightAttitude);

private:
  OPERATION_TYPE _operationType;
  QRect _rect;
  qreal _factor = 1.0;
  qreal _heightAttitude;
  int _dx = 1;
  int _dy = 1;
};
} // namespace EventOperation
