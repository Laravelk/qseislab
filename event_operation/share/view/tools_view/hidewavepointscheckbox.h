#pragma once

#include <QFrame>
#include <QCheckBox>


namespace  EventOperation {
    class HideWavePointsWidget : public QFrame
    {
        Q_OBJECT
    public:
        enum Wave { PWAVE, SWAVE };
        enum State { Unchecked, Checked };

        HideWavePointsWidget(QWidget *parent = nullptr);
    signals:
        void updateChecked(Wave, State) const;
    private:
        QCheckBox *pWave;
        QCheckBox *sWave;
    };
}

