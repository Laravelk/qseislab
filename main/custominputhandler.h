#pragma once

#include <QtDataVisualization>

using namespace QtDataVisualization;

namespace Main {
    class CustomInputHandler : public QAbstract3DInputHandler
    {
        Q_OBJECT
    public:
        explicit CustomInputHandler(QObject *parent = nullptr);

        virtual void mouseMoveEvent(QMouseEvent *event, const QPoint &mousePos);
        virtual void wheelEvent(QWheelEvent *event);
    };
}
