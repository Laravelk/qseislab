#include "filemanager.h"

#include <QHBoxLayout>

namespace EventOperation {
namespace AddEvent {
FileManager::FileManager(QWidget *parent)
    : QFrame(parent), _label(new QLabel("Segy file: ", this)),
      _fileName(new QLabel("<test.segy>", this)),
      _browseButton(new QPushButton("Browse", this)),
      _fileDialog(new QFileDialog(this)) {

  setFixedWidth(250);
  setFrameStyle(1);

  _fileName->setMinimumWidth(100);
  _fileName->setFrameStyle(1);
  // use QFontMetrics

  _fileDialog->setFileMode(QFileDialog::ExistingFile);
  _fileDialog->setOption(QFileDialog::DontResolveSymlinks);
  _fileDialog->setNameFilter("*.segy, *.sgy");
  //    _fileDialog->setDirectory(QDir::home());

  connect(_browseButton, &QPushButton::clicked, [this] {
    _fileDialog->open(this, SLOT(recvFilePath(const QString &)));
  });

  QHBoxLayout *layout = new QHBoxLayout();
  layout->addWidget(_label);
  layout->addWidget(_fileName, 1);
  layout->addWidget(_browseButton);
  setLayout(layout);
}

void FileManager::clear() { _fileName->clear(); }

void FileManager::recvFilePath(const QString &path) {
  _fileName->setText(QFileInfo(path).fileName());

  emit sendFilePath(path);
}

} // namespace AddEvent
} // namespace EventOperation
