#include "scatterdatamodifier.h"

#include <QtDataVisualization/QScatterDataProxy>
#include <QtDataVisualization/QValue3DAxis>
#include <QtDataVisualization/Q3DScene>
#include <QtDataVisualization/Q3DCamera>
#include <QtDataVisualization/QScatter3DSeries>
#include <QtDataVisualization/Q3DTheme>
#include <QtCore/qmath.h>
#include <QtCore/QTextStream>
#include <QtCore/QDebug>

using namespace QtDataVisualization;

namespace Main {

    ScatterDataModifier::ScatterDataModifier(Q3DScatter *scatter)
        : _graph(scatter),
          _inputHandler(new CustomInputHandler())
    {
        _graph->activeTheme()->setType(Q3DTheme::ThemeDigia);
        _graph->setShadowQuality(QAbstract3DGraph::ShadowQualityMedium);
        _graph->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPresetFront);

        _graph->setAxisX(new QValue3DAxis);
        _graph->setAxisY(new QValue3DAxis);
        _graph->setAxisZ(new QValue3DAxis);

        _graph->axisX()->setRange(-10.0f, 10.0f);
        _graph->axisY()->setRange(-5.0f, 5.0f);
        _graph->axisZ()->setRange(-5.0f, 5.0f);

        QScatter3DSeries *series = new QScatter3DSeries;
        series->setItemLabelFormat(QStringLiteral("@xLabel, @yLabel, @zLabel"));
        series->setMesh(QAbstract3DSeries::MeshCube);
        series->setItemSize(0.15f);
        _graph->addSeries(series);

        _animationCameraX = new QPropertyAnimation
                (_graph->scene()->activeCamera(), "xRotation");
        _animationCameraX->setDuration(20000);
        _animationCameraX->setStartValue(QVariant::fromValue(0.0f));
        _animationCameraX->setEndValue(QVariant::fromValue(360.0f));
        _animationCameraX->setLoopCount(-1);


        QPropertyAnimation *upAnimation = new QPropertyAnimation
                (_graph->scene()->activeCamera(), "yRotation");
        upAnimation->setDuration(9000);
        upAnimation->setStartValue(QVariant::fromValue(5.0f));
        upAnimation->setEndValue(QVariant::fromValue(45.0f));

        QPropertyAnimation *downAnimation = new QPropertyAnimation
                (_graph->scene()->activeCamera(), "yRotation");
        downAnimation->setDuration(9000);
        downAnimation->setStartValue(QVariant::fromValue(45.0f));
        downAnimation->setEndValue(QVariant::fromValue(5.0f));

        _animationCameraY = new QSequentialAnimationGroup();
        _animationCameraY->setLoopCount(-1);
        _animationCameraY->addAnimation(upAnimation);
        _animationCameraY->addAnimation(downAnimation);

        _animationCameraX->start();
        _animationCameraY->start();

    //    _graph->setActiveInputHandler(_inputHandler);

        _selectionTimer = new QTimer(this);
        _selectionTimer->setInterval(10);
        _selectionTimer->setSingleShot(false);
        QObject::connect(_selectionTimer, &QTimer::timeout, this,
                         &ScatterDataModifier::triggerSelection);
        _selectionTimer->start();
    }

    ScatterDataModifier::~ScatterDataModifier()
    {
        delete _graph;
    }

    void ScatterDataModifier::start()
    {
        addData();
    }

    void ScatterDataModifier::addData()
    {
        QVector<QVector3D> itemList;

        // Read data items from the file to QVector
        QTextStream stream;
        QFile dataFile(":/data/data.txt");
        if (dataFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            stream.setDevice(&dataFile);
            while (!stream.atEnd()) {
                QString line = stream.readLine();
                if (line.startsWith("#")) // Ignore comments
                    continue;
                QStringList strList = line.split(",", QString::SkipEmptyParts);
                // Each line has three data items: xPos, yPos and zPos value
                if (strList.size() < 3) {
                    qWarning() << "Invalid row read from data:" << line;
                    continue;
                }
                itemList.append(QVector3D(
                                     strList.at(0).trimmed().toFloat(),
                                     strList.at(1).trimmed().toFloat(),
                                     strList.at(2).trimmed().toFloat()));
            }
        } else {
            qWarning() << "Unable to open data file:" << dataFile.fileName();
        }

        // Add data from the QVector to datamodel
        QScatterDataArray *dataArray = new QScatterDataArray;
        dataArray->resize(itemList.count());
        QScatterDataItem *ptrToDataArray = &dataArray->first();
        for (int i = 0; i < itemList.count(); i++) {
            ptrToDataArray->setPosition(itemList.at(i));
            ptrToDataArray++;
        }

        _graph->seriesList().at(0)->dataProxy()->resetArray(dataArray);
    }

    void ScatterDataModifier::toggleCameraAnimation()
    {
        if (_animationCameraX->state() != QAbstractAnimation::Paused) {
            _animationCameraX->pause();
            _animationCameraY->pause();
        } else {
            _animationCameraX->resume();
            _animationCameraY->resume();
        }
    }

    void ScatterDataModifier::triggerSelection()
    {
        _graph->scene()->setSelectionQueryPosition(_inputHandler->inputPosition());
    }

    void ScatterDataModifier::shadowQualityUpdatedByVisual(QAbstract3DGraph::ShadowQuality sq)
    {
        int quality = int(sq);
        emit shadowQualityChanged(quality); // connected to a checkbox in main.cpp
    }

    void ScatterDataModifier::changeShadowQuality(int quality)
    {
        QAbstract3DGraph::ShadowQuality sq = QAbstract3DGraph::ShadowQuality(quality);
        _graph->setShadowQuality(sq);
    }
}
