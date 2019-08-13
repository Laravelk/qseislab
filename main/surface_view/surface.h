#pragma once

#include <QtDataVisualization>
#include <QtWidgets>
#include <Q3DSurface>

using namespace QtDataVisualization;

namespace Main {
class Surface : public QObject {
    Q_OBJECT
public:
    explicit Surface(Q3DSurface *surface);
    ~Surface();
private:
    Q3DSurface* _surface;
    QCustom3DItem* _previousAnimatedItem;
    QVector3D* _previousScaling;
};
} // namespace Main
