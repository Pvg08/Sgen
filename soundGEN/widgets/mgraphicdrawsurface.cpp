#include "mgraphicdrawsurface.h"


double MGraphicDrawSurface::getT() const
{
    return t;
}

void MGraphicDrawSurface::setT(double value)
{
    t = value;
}

double MGraphicDrawSurface::getT0() const
{
    return t0;
}

void MGraphicDrawSurface::setT0(double value)
{
    t0 = value;
}

double MGraphicDrawSurface::getAmp() const
{
    return amp;
}

void MGraphicDrawSurface::setAmp(double value)
{
    amp = value;
}

double MGraphicDrawSurface::getFreq() const
{
    return freq;
}

void MGraphicDrawSurface::setFreq(double value)
{
    freq = value;
}

double MGraphicDrawSurface::getDt() const
{
    return dt;
}

void MGraphicDrawSurface::setDt(double value)
{
    dt = value;
    calculateTGrid();
}

double MGraphicDrawSurface::getKt() const
{
    return kt;
}

void MGraphicDrawSurface::setKt(double value)
{
    kt = value;
}

double MGraphicDrawSurface::getKamp() const
{
    return kamp;
}

void MGraphicDrawSurface::setKamp(double value)
{
    kamp = value;
}

double MGraphicDrawSurface::getDt_axis() const
{
    return dt_axis;
}

void MGraphicDrawSurface::incT()
{
    t += kt*dt;
}

void MGraphicDrawSurface::setGraphicFunction(const GenSoundFunction &value)
{
    graphicFunction = value;
}

MGraphicDrawSurface::MGraphicDrawSurface() :
    QWidget()
{
}

void MGraphicDrawSurface::calculateTGrid()
{
    double cl_dt = dt;

    if (cl_dt>0)
    {
        double divider = 1;
        while (cl_dt<1) {
            divider *= 10;
            cl_dt *= 10;
        }
        cl_dt = round(cl_dt);
        if (cl_dt>5) dt_axis = 5/divider;
        else if(cl_dt>2) dt_axis = 2/divider;
        else if(cl_dt>1) dt_axis = 1/divider;
        else dt_axis = 0.5/divider;
    } else
    {
        dt_axis = 1;
    }
}

void MGraphicDrawSurface::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);

    QPainter painter(this);

    painter.setBackground(QBrush(Qt::white));
    painter.eraseRect(rect());
    if (!graphicFunction) return;

    int points_count = width() - 1;
    int height_center = height() / 2;
    double k_y_graphic = kamp * amp * 0.375 * height();
    double k_t_graphic = dt/points_count;
    double kFreq = freq*2.0*M_PI;

    int i;
    double x0, y0, x1, y1;

    double t_axis = floor(t/dt_axis) * dt_axis;
    double next_t = t+dt;


    painter.setPen(Qt::black);
    painter.drawLine(0,height_center,points_count,height_center);

    do {
        t_axis+=dt_axis;
        x0 = (t_axis-t)/k_t_graphic;
        painter.drawLine(x0,0,x0,height_center*2);
        painter.drawText(x0+5, height_center+5, QString::number(t_axis, 'f', 4));
    } while (t_axis<=next_t);

    painter.setPen(QPen(QBrush(Qt::red), 2));

    x1 = 0;
    y1 = height_center - k_y_graphic*graphicFunction(t, kFreq, freq, base_play_sound);
    for(i=1;i<points_count;i++) {
        x0 = x1;
        y0 = y1;
        x1 = i;
        y1 = height_center - k_y_graphic*graphicFunction(t+i*k_t_graphic, kFreq, freq, base_play_sound);
        painter.drawLine(x0,y0,x1,y1);
    }
}