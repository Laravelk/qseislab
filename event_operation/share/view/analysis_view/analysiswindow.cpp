#include "analysiswindow.h"

namespace EventOperation {

AnalysisWindow::AnalysisWindow(std::shared_ptr<Data::SeismEvent> &event, QWidget *parent) : QFrame(parent)
{
    this->setMinimumSize(1000, 200);
    mainLayout = new QHBoxLayout;
    analysis = new PolarizationAnalysisWindow(event);
    mainLayout->addWidget(analysis);
    graph = new PolarGraph(new QPolarChart);
    mainLayout->addWidget(graph);
    graph->update(event.get());

    setLayout(mainLayout);
}

}
