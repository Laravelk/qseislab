#pragma once

#include "seism_data_type.h"
#include "seismcomponent.h"
#include <QDateTime>
#include <QDir>
#include <QJsonObject>
#include <QUuid>

#include <memory>
#include <set>

namespace Data {
// class SeismWell;
class SeismReceiver;
class SeismEvent : public QObject {
  Q_OBJECT
public:
  class Info {
  public:
    explicit Info();

    void setName(const QString &);
    const QString &getName() const;

    void setType(int);
    int getType() const;

    void setAddedDateTime(const QDateTime &);
    const QDateTime &getAddedDateTime() const;

    void setStampDateTime(const QDateTime &);
    const QDateTime &getStampDateTime() const;

  private:
    QString name;
    int type = 0;
    QDateTime addedDateTime;
    QDateTime stampDateTime;

    friend class SeismEvent;
  };

  static const QString _default_path;

  enum TransformOperation {
    RotateData,
    TestMultiplier,
    MovePick,
    RemovePick,
    AddPick,
    FFilteringData
  };

  explicit SeismEvent();
  explicit SeismEvent(
      const QJsonObject &,
      //                      std::map<QUuid, std::shared_ptr<SeismWell>> &,
      const std::list<std::shared_ptr<SeismReceiver>> &,
      const QDir &) noexcept(false);

  explicit SeismEvent(const SeismEvent &);

  const QUuid &getUuid() const;

  void changeTrigger() const;

  /////// all-info
  void setInfo(const Info &);
  const Info &getInfo() const;

  const QString &getName() const;
  const QDateTime &getAddedDateTime() const;
  int getType() const; // TODO: remove
  const QDateTime &getStampDateTime() const;
  /////// all-info end

  int getPickAmountByType(const SeismWavePick::Type) const;
  int getReceiverAmount() const;

  int getComponentAmount() const;
  void addComponent(const std::shared_ptr<SeismComponent> &);
  bool removeComponentByReceiverUuid(const QUuid &);
  //  const std::list<std::shared_ptr<SeismComponent>> &getComponents() const;
  const std::vector<SeismComponent const *> getComponents() const;
  const std::vector<SeismComponent *> getComponents();

  //    bool isTransformBy(TransformOperation) const;

  void process();
  bool isProcessed() const;
  const Point &getLocation() const;

  QJsonObject &writeToJson(QJsonObject &, const QDir &) noexcept(false);

signals:
  void infoChanged(SeismEvent const *) const;
  void dataChanged(SeismEvent const *) const;

private:
  QUuid _uuid;
  QString _path;

  Info _info;

  bool _isProcessed{false};
  Point _location{0, 0, 0};

  std::vector<std::shared_ptr<SeismComponent>> _components;

  //  std::set<TransformOperation>
  //      _appliedOperations; // NOTE: уместно ли использовать set?
  //  void addTransformOperation(TransformOperation);
  //  void removeTransformOperation(TransformOperation);

  //  friend class EventOperation::Modefication::RotateDataToEBasis;
};

} // namespace Data
