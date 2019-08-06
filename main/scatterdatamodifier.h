#pragma once

#include "custominputhandler.h"

#include <QtDataVisualization/q3dscatter.h>
#include <QtGui/QFont>
#include <QtCore/QTimer>
#include <QtCore/QPropertyAnimation>
#include <QtCore/QSequentialAnimationGroup>
#include <QtGui/QVector3D>

using namespace QtDataVisualization;

namespace Main {
    class ScatterDataModifier : public QObject
    {
        Q_OBJECT
    public:
        explicit ScatterDataModifier(Q3DScatter *scatter);
        ~ScatterDataModifier();

        void addData();
        void toggleCameraAnimation();
        void start();

    public Q_SLOTS:
        void changeShadowQuality(int quality);
        void shadowQualityUpdatedByVisual(QAbstract3DGraph::ShadowQuality shadowQuality);
        void triggerSelection();

    Q_SIGNALS:
        void shadowQualityChanged(int quality);

    private:
        Q3DScatter *_graph;
        QPropertyAnimation *_animationCameraX;
        QSequentialAnimationGroup *_animationCameraY;
        CustomInputHandler *_inputHandler;
        QTimer *_selectionTimer;
    };
}
