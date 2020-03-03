#pragma once

#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPolarChart>

#include "../hodogram/polarizationanalysiswindow.h"
#include "../polar_graph/polargraph.h"

namespace Data {
class SeismEvent;
class SeismComponent;
class SeismTrace;
} // namespace Data

namespace EventOperation {
class ChangeBorderOnPolarWidget;
class HideWavePointsWidget;
class AnalysisWindow : public QFrame
{
    Q_OBJECT
public:
    AnalysisWindow(std::shared_ptr<Data::SeismEvent> &event, QWidget *parent = nullptr);
    void updatePolarGraph(const Data::SeismEvent *const);
    void loadEvent(std::shared_ptr<Data::SeismEvent> &event);
    void setHodogramToDefault();
    void removePick(int numberOfReceiver, Data::SeismWavePick::Type type);
private:
    QHBoxLayout *_mainLayout;
    QVBoxLayout *_polarGraphTools;
    PolarizationAnalysisWindow *_hodogram = nullptr;
    PolarGraph *_graph = nullptr;
    ChangeBorderOnPolarWidget *_changeBorderOnPolarWidget;
    HideWavePointsWidget *_hidePointsWidget;
    Data::SeismEvent const *_event;

    void init(std::shared_ptr<Data::SeismEvent> &);
};
}

