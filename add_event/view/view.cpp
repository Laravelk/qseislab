#include "view.h"

#include "infoevent.h"
#include "filemanager.h"
#include "simulationgraphic.h" // TODO: need to remove
#include "graphicevent.h"

#include <QBoxLayout>
#include <QFileDialog>
#include <QLabel>
#include <QMessageBox>


typedef Data::SeismEvent SeismEvent;


namespace AddEvent {
View::View(QWidget *parent)
    :QDialog(parent),
      _fileManager(new FileManager(this)),
      _infoEvent(new InfoEvent(this)),
      _graphicEvent(new GraphicEvent(this)),
      _addButton(new QPushButton("Add", this)),
      _cancelButton(new QPushButton("Cancel",this)),
      _scrollArea(new QScrollArea())
{

    setWindowTitle("SeismWindow");
    setMinimumSize(1100,590);


    connect(_fileManager, SIGNAL(sendFilePath(const QString& )), this, SLOT(recvFilePath(const QString& )));


    _addButton->setDisabled(true);
    connect(_addButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(_cancelButton, SIGNAL(clicked()), this, SLOT(reject()));


    QVBoxLayout* leftLayout = new QVBoxLayout();
    leftLayout->addWidget(_fileManager);
    leftLayout->addWidget(_infoEvent);
    leftLayout->addStretch(1);

    QHBoxLayout* buttonsLayout = new QHBoxLayout();
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(_addButton);
    buttonsLayout->addWidget(_cancelButton);

    QVBoxLayout *graphicLayout = new QVBoxLayout();
    graphicLayout->addWidget(_graphicEvent->getView(), 3);
    graphicLayout->addStretch(1);
    graphicLayout->addLayout(buttonsLayout);

    QHBoxLayout* mainLayout = new QHBoxLayout();
    mainLayout->addLayout(leftLayout, 2);
    mainLayout->addStretch(1);
    mainLayout->addLayout(graphicLayout);

    setLayout(mainLayout);
}

void View::update(const std::unique_ptr<Data::SeismEvent>& event)
{
    if(!event) {
        _fileManager->clear();
        _infoEvent->clear();
        _graphicEvent->clear();
        _addButton->setDisabled(true);
        return;
    }
    _infoEvent->update(event);
    _graphicEvent->update(event);
    _addButton->setDisabled(false);
}

void View::setNotification(const QString& text)
{
    QMessageBox* msg = new QMessageBox(this);
    msg->setWindowTitle("Message");
    msg->addButton(QMessageBox::StandardButton::Ok);
    msg->setText(text);
    msg->exec();
}

void View::recvFilePath(const QString& path)
{
    emit sendFilePath(path);
}


} // namespace AddEvent
