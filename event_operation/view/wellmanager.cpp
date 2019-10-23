#include "wellmanager.h"

#include "filemanager.h"

#include <QBoxLayout>

namespace EventOperation {
namespace Generic {
WellManager::WellManager(const std::map<QUuid, QString> &wellNames_map,
                         QWidget *parent)
    : QFrame(parent), _comboBox(new QComboBox(this)),
      _fileManager(new FileManager(this)) {

  // Setting`s
  setFrameStyle(1);
  _fileManager->setDisabled(true);

  _comboBox->addItem("Select a well name...");
  for (auto &uuid_name : wellNames_map) {
    _comboBox->addItem(uuid_name.second, uuid_name.first);
  }

  QPushButton *removeButton = new QPushButton("Remove");
  // Setting`s end

  // Connecting
  connect(_comboBox, QOverload<int>::of(&QComboBox::activated),
          [this](auto index) {
            if (0 == index) {
              _wellUuid = QUuid();
              _fileManager->setDisabled(true);
            } else {
              _wellUuid = _comboBox->itemData(index).toUuid();
              _fileManager->setEnabled(true);
            }
          });
  connect(_fileManager, &FileManager::sendFilePath, [this](auto &path) {
    _comboBox->setDisabled(true);
    emit sendWellUuidAndFilePath({_wellUuid, path});
  });

  connect(removeButton, &QPushButton::clicked,
          [this]() { emit removeClicked(this, _wellUuid); });
  // Connecting end

  // Layout`s
  QHBoxLayout *buttonLayout = new QHBoxLayout();
  buttonLayout->addStretch(1);
  buttonLayout->addWidget(removeButton);

  QVBoxLayout *mainLayout = new QVBoxLayout();
  mainLayout->addWidget(_comboBox);
  mainLayout->addWidget(_fileManager);
  mainLayout->addStretch(1);
  mainLayout->addLayout(buttonLayout);

  setLayout(mainLayout);
  // Layout`s end
}

void WellManager::updateWellNames(
    const std::map<QUuid, QString> &wellNames_map) {
  QString def_text = _comboBox->itemText(0);
  _comboBox->clear();
  _comboBox->addItem(def_text);
  for (auto &uuid_name : wellNames_map) {
    _comboBox->addItem(uuid_name.second, uuid_name.first);
  }
}

void WellManager::clear() { _fileManager->clear(); }

} // namespace Generic
} // namespace EventOperation
