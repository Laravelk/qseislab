#include "filemanager.h"

#include <QHBoxLayout>


namespace AddEvent {
FileManager::FileManager(QWidget *parent)
    :QFrame(parent),
     _label(new QLabel("Segy file: ", this)),
     _fileName(new QLabel("<test.segy>", this)),
     _browseButton(new QPushButton("Browse", this)),
     _fileDialog(new QFileDialog(this))
{
    setFixedWidth(250);
    setFrameStyle(1);


    _fileName->setMinimumWidth(100);
    _fileName->setFrameStyle(1);
    // use QFontMetrics


    _fileDialog->setFileMode(QFileDialog::ExistingFile);
    _fileDialog->setOption(QFileDialog::DontResolveSymlinks);
    _fileDialog->setNameFilter("*.segy, *.sgy");
//    _fileDialog->setDirectory(QDir::home());
    connect(_fileDialog, SIGNAL(fileSelected(const QString& )), this, SLOT(recvFilePath(const QString& )));


    connect(_browseButton, SIGNAL(clicked()), _fileDialog, SLOT(open()));


    QHBoxLayout* layout = new QHBoxLayout();
    layout->addWidget(_label);
    layout->addWidget(_fileName);
    layout->addWidget(_browseButton);
    layout->addStretch(1);
    setLayout(layout);
}

void FileManager::clear()
{
    _fileName->clear();
}

void FileManager::recvFilePath(const QString& path)
{
    QStringList parts = path.split("/");
    QString lastBit = parts.at(parts.size()-1);
    _fileName->setText(lastBit);

    emit sendFilePath(path);
}


} // namespace AddEvent
