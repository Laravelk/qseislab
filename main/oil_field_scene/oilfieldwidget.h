#ifndef OILFIELDWIDGET_H
#define OILFIELDWIDGET_H

#include "oilFieldScene.h"

#include <QWidget>



namespace Main {
class OilFieldWidget : public QWidget {
    Q_OBJECT
public:
    explicit OilFieldWidget(QWidget* parent = nullptr);

    void setProject(Data::SeismProject const * const);

    void addEvent(Data::SeismEvent const * const);
    void addHorizon(Data::SeismHorizon const * const);
    void addReceiver(Data::SeismReceiver const * const);
    void addWell(Data::SeismWell const * const);

    bool removeEvent(const Uuid &);
    bool removeHorizon(const Uuid &);
    bool removeReceiver(const Uuid &);
    bool removeWell(const Uuid &);

    void hideSeismEvent(const QUuid& );
    void showSeismEvent(const QUuid& );

private:
    OilFieldScene *_oilFieldScene;


    Q3DSurface *_graph;
    QCheckBox *_eventBox;
    QCheckBox *_receiverBox;
    QCheckBox *_wellBox;
    QCheckBox *_horizonBox;
};

}
#endif // OILFIELDWIDGET_H
