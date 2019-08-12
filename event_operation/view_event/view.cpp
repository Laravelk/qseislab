#include "view.h"

#include "event_operation/share_view/infoevent.h"
#include "event_operation/share_view/graphicevent.h"

#include <QBoxLayout>
#include <QPushButton>


namespace EventOperation {
namespace ViewEvent {
View::View(QWidget* parent)
    :QDialog(parent),
     _infoEvent(new InfoEvent(this)),
     _graphicEvent(new GraphicEvent(this)),
     _okButton(new QPushButton("Ok",this))
{
    setWindowTitle("SeismWindow");
    setMinimumSize(1100,590);


    connect(_okButton, SIGNAL(clicked()), this, SLOT(accept()));


    QVBoxLayout* leftLayout = new QVBoxLayout();
    leftLayout->addWidget(_infoEvent);
    leftLayout->addStretch(1);

    QHBoxLayout* buttonsLayout = new QHBoxLayout();
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(_okButton);

    QVBoxLayout* graphicLayout = new QVBoxLayout();
    graphicLayout->addWidget(_graphicEvent->getView(), 10);
    graphicLayout->addStretch(1);
    graphicLayout->addLayout(buttonsLayout);

    QHBoxLayout* mainLayout = new QHBoxLayout();
    mainLayout->addLayout(leftLayout);
//    mainLayout->addStretch(1);
    mainLayout->addLayout(graphicLayout, 10);

    setLayout(mainLayout);
}

void View::update(const std::unique_ptr<Data::SeismEvent>& event)
{
    if(!event) {
        _infoEvent->clear();
        _graphicEvent->clear();
        return;
    }
    _infoEvent->update(event);
    _graphicEvent->update(event);
}


} // namespace ViewEvent
} // namespace EventOperation
