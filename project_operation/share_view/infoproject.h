#pragma once

#include <QFrame>
#include <QLineEdit>


namespace ProjectOperation {
class InfoProject : public QFrame {
public:
    typedef enum {
        CLEAN,
        DEFAULT
    } MODE;

    explicit InfoProject(MODE, QWidget* parent = nullptr);

    void setName(const QString& );
    QString getName() const;

    void setDate(const QDate& );
    QDate getDate() const;

    void setTime(const QTime& );
    QTime getTime() const;

    void clear();

private:
    QLineEdit* _nameLineEdit;
    QLineEdit* _dateLineEdit;
    QLineEdit* _timeLineEdit;

    // NOTE: можно в приватном поле хранить форматы даты и времени
};


} // namespace ProjectOperation
