#pragma once

#include "data/seismwavepick.h"

#include <QBoxLayout>
#include <QComboBox>
#include <QDialog>
#include <QFileDialog>
#include <QListWidget>
#include <QUuid>

#include <memory>
#include <set>

namespace Data {
class SeismEvent;
class SeismWell;
} // namespace Data

namespace EventOperation {
class InfoEvent;
class GraphicController;
class ChartGesture;
namespace MoreEvents {
class View : public QDialog {
  Q_OBJECT

public:
  explicit View(const std::set<QString> &, const std::map<QUuid, QString> &,
                QWidget *parent = nullptr);

  void loadEvent(const std::unique_ptr<Data::SeismEvent> &);
  void unloadEvent();

  void update(const std::map<QUuid, std::unique_ptr<Data::SeismEvent>> &);

  void setNotification(const QString &);
  void settingEventInfo(const std::unique_ptr<Data::SeismEvent> &) const;
  ChartGesture *getChartGesture();

signals:
  void hideCurrentEvent();
  void changeCurrentEvent(const QUuid &);
  void removeEvent(const QUuid &);

  void sendWellUuidAndFilePaths(const QUuid &, const QStringList &) const;

  void sendPicksInfo(Data::SeismWavePick::Type, int, int, int, int);
  void createPolarizationAnalysisWindow();

private slots:
  void recvFilesPath(const QStringList &);

private:
  void addLocal(const QString &);
  void removeLocal(const QString &);
  QBrush updateRepetition(const QString &);
  bool allValid() const;

  InfoEvent *_infoEvent;
  QComboBox *_wellNames;
  QUuid _wellUuid;
  QFileDialog *_fileDialog;

  QListWidget *_eventList;

  GraphicController *_graphicEvent;
  QPushButton *_okButton;
  QPushButton *_cancelButton;

  std::set<QString> _globalEventNames;
  std::map<QString, int> _localEventNames;
};

} // namespace MoreEvents
} // namespace EventOperation