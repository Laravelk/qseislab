#include "eventtoolswidget.h"

#include <QBoxLayout>
#include <QPushButton>

namespace EventOperation {
EventToolsWidget::EventToolsWidget(QWidget *parent) : QWidget(parent) {

  // Setting`s
  QPushButton *dataToEBasisButton = new QPushButton("Data to E-basis");
  // Setting`s end

  // Connecting
  connect(dataToEBasisButton, &QPushButton::clicked,
          [this]() { emit dataToEBasisClicked(); });
  // Connecting end

  // Layout`s
  QHBoxLayout *mainLayout = new QHBoxLayout();
  mainLayout->addWidget(dataToEBasisButton);

  setLayout(mainLayout);
  // Layout`s end
}

} // namespace EventOperation
