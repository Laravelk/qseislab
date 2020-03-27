#pragma once

#include <QObject>
#include <QMessageBox>

#include <set>

namespace Data {
class SeismEvent;
}

namespace EventOperation {
namespace SaveEvent {
class Controller : public QObject{
    Q_OBJECT
public:
    explicit Controller(QObject* parent = nullptr);

    void save(const std::set<Data::SeismEvent*>& );

//    void save(Data::SeismEvent const * const) const;

signals:
    void finished() const;

private:
    void recvDirPath(const QString &);
    void finish();
    static void setNotification(const QString &,
                                QMessageBox::Icon icon = QMessageBox::Critical);

    std::set<Data::SeismEvent*> _events;
};

}
}


