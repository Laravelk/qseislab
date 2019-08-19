#include "filemanager.h"

#include <QBoxLayout>

namespace HorizonOperation {
FileManager::FileManager(QWidget *parent)
    : QFrame(parent), _label(new QLabel("Horizon file: ", this)),
      _fileName(new QLabel(this)),
      _browseButton(new QPushButton("Browse", this)),
      _fileDialog(new QFileDialog(this)) {
  setFrameStyle(1);

  _fileName->setMinimumWidth(100);
  _fileName->setFrameStyle(1);

  _fileDialog->setFileMode(QFileDialog::ExistingFile);
  _fileDialog->setOption(QFileDialog::DontResolveSymlinks);
  _fileDialog->setNameFilter("*.bin");
  connect(_fileDialog, SIGNAL(fileSelected(const QString &)), this,
          SLOT(recvFilePath(const QString &)));

  connect(_browseButton, SIGNAL(clicked()), _fileDialog, SLOT(open()));

  QHBoxLayout *layout = new QHBoxLayout();
  layout->addWidget(_label);
  layout->addWidget(_fileName, 1);
  layout->addWidget(_browseButton);
  setLayout(layout);
}

void FileManager::clear() { _fileName->clear(); }

void FileManager::recvFilePath(const QString &path) {
  QFileInfo fileInfo(path);
  _fileName->setText(fileInfo.fileName());

  emit sendFilePath(path);
}

} // namespace HorizonOperation
