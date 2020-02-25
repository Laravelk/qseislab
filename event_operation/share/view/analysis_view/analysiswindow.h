#pragma once

#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPolarChart>

#include "../3dscene/polarizationanalysiswindow.h"
#include "../polar_graph/polargraph.h"

namespace EventOperation {
class AnalysisWindow : public QFrame
{
    Q_OBJECT
public:
    AnalysisWindow(std::shared_ptr<Data::SeismEvent> &event, QWidget *parent = nullptr);

private:
    QHBoxLayout *mainLayout;
    PolarizationAnalysisWindow *analysis = nullptr;
    PolarGraph *graph = nullptr;
};
}

