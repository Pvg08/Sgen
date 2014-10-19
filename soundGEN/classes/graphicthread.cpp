#include "graphicthread.h"
#include <QtCore>
#include <QtGui>

graphicThread::graphicThread(QObject *parent) :
    QThread(parent)
{
    Stop = false;
    linksCount = 0;
}

void graphicThread::run()
{
    do {
        emit DrawStep();
        this->msleep(30);
    } while (!Stop);
    emit DrawStep();
}

void graphicThread::addGraphic(QObject *graphicDrawer)
{
    linksCount++;
    connect(this, SIGNAL(DrawStep()), graphicDrawer, SLOT(drawCycle()));
}

void graphicThread::removeGraphic()
{
    if (linksCount>0) linksCount--;
}

int graphicThread::getLinksCount() const
{
    return linksCount;
}

