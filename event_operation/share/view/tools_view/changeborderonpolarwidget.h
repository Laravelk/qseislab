#pragma once

#include <QFrame>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

namespace EventOperation {
class ChangeBorderOnPolarWidget : public QFrame
{
    Q_OBJECT
public:
    ChangeBorderOnPolarWidget(QWidget *parent = nullptr);

    int getAngularMin();
    int getAngularMax();

    int getRadialMin();
    int getRadialMax();

signals:
    void valueChanged();

private:
    QLineEdit *_angularMin;
    QLineEdit *_angularMax;
    QLineEdit *_radialMin;
    QLineEdit *_radialMax;

    QLabel *_angularLabel;
    QLabel *_minAngularLabel;
    QLabel *_maxAngularLabel;
    QLabel *_radialLabel;
    QLabel *_minRadialLabel;
    QLabel *_maxRadialLabel;

    bool _angularMinChanged = false;
    bool _angularMaxChanged = false;
    bool _radialMinChanged = false;
    bool _radialMaxChanged = false;

    QPushButton *_applyButton;
};
}
