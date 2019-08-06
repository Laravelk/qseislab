#include "filemanager.h"

#include <QBoxLayout>


namespace ProjectOperation {
FileManager::FileManager(QWidget *parent)
    :QFrame(parent),
     _label(new QLabel("File: ", this)),
     _fileName(new QLabel(this)),
     _button(new QPushButton("Browse", this)),
     _fileDialog(new QFileDialog(this))
{
    setFrameStyle(1);

    _fileName->setMinimumWidth(100);
    _fileName->setFrameStyle(1);

    _fileDialog->setFileMode(QFileDialog::ExistingFile);
    _fileDialog->setOption(QFileDialog::DontResolveSymlinks);
    _fileDialog->setNameFilter("*.json");
//    _fileDialog->setDirectory(QDir::home());

    connect(_button, SIGNAL(clicked()), _fileDialog, SLOT(open()));
    connect(_fileDialog, SIGNAL(fileSelected(const QString& )), this, SLOT(recvFilePath(const QString& )));
//    _fileDialog->open(); // NOTE: можно ли так сделать и будет ли это правильно? (чтобы диалог сразу появлялось)

    QHBoxLayout* layout = new QHBoxLayout();
    layout->addWidget(_label);
    layout->addWidget(_fileName,1);
    layout->addWidget(_button);
    setLayout(layout);
}

void FileManager::clear()
{
    _fileName->clear();
}

void FileManager::recvFilePath(const QString& path)
{
    QFileInfo fileInfo(path);
    _fileName->setText(fileInfo.fileName());

    emit sendFilePath(path);
}


} // namespace ProjectOperation
