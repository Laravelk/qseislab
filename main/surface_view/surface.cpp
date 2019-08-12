#include "surface.h"

namespace Main {
Surface::Surface(Q3DSurface *surface)
                : _surface(surface)
{
   _surface->setFlags(surface->flags() ^ Qt::FramelessWindowHint);

    QSurfaceDataArray *data = new QSurfaceDataArray;
    QSurfaceDataRow *dataRow1 = new QSurfaceDataRow;
     QSurfaceDataRow *dataRow2 = new QSurfaceDataRow;

    *dataRow1 << QVector3D(0.0f, 0.1f, 0.5f) << QVector3D(1.0f, 0.5f, 0.5f);
    *dataRow2 << QVector3D(0.0f, 1.8f, 1.0f) << QVector3D(1.0f, 1.2f, 1.0f);
    *data << dataRow1 << dataRow2;

    QSurface3DSeries *series = new QSurface3DSeries;
    series->dataProxy()->resetArray(data);
    surface->addSeries(series);
    surface->show();
}

Surface::~Surface()
{
    delete _surface;
}


}
