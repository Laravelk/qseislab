#include "filemanager.h"

#include <QBoxLayout>

namespace WellOperation {
FileManager::FileManager(QWidget *parent)
    : QFrame(parent), _label(new QLabel("Well file: ", this)),
      _fileName(new QLabel(this)),
      _browseButton(new QPushButton("Browse", this)),
      _fileDialog(new QFileDialog(this)) {

  // Setting`s
  setFrameStyle(1);

  _fileName->setMinimumWidth(100);
  _fileName->setFrameStyle(1);

  _fileDialog->setFileMode(QFileDialog::ExistingFile);
  _fileDialog->setOption(QFileDialog::DontResolveSymlinks);
  _fileDialog->setNameFilter("*.txt");
  // Setting`s  end

  // Connecting
  connect(_browseButton, &QPushButton::clicked, [this] {
    _fileDialog->open(this, SLOT(recvFilePath(const QString &)));
  });
  // Connecting end

  // Layout`s
  QHBoxLayout *layout = new QHBoxLayout();
  layout->addWidget(_label);
  layout->addWidget(_fileName, 1);
  layout->addWidget(_browseButton);
  setLayout(layout);
  // Layout`s end
}

void FileManager::clear() { _fileName->clear(); }

void FileManager::recvFilePath(const QString &path) {
  _fileName->setText(QFileInfo(path).fileName());
  emit sendFilePath(path);
}

} // namespace WellOperation
