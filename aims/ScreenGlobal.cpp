#include "ScreenGlobal.h"
#include <QGuiApplication>

using namespace aim;

ScreenGlobal::ScreenGlobal(QObject *parent):
    QObject(parent), screen(*QGuiApplication::primaryScreen())
{}


int ScreenGlobal::getScreenWidth() const {
#ifdef Q_OS_ANDROID
    return screen.geometry().width();
#else
   return 480;
#endif
}


int ScreenGlobal::getScreenHeight() const {
#ifdef Q_OS_ANDROID
    return screen.geometry().height();
#else
    return 800;
#endif
}


double ScreenGlobal::adaptXSize(const int xValue) const {
#ifdef Q_OS_ANDROID
    return xValue/480.0*screen.geometry().width();
#else
    return xValue;
#endif
}


double ScreenGlobal::adaptYSize(const int yValue) const {
#ifdef Q_OS_ANDROID
    return yValue/800.0*screen.geometry().height();
#else
    return yValue;
#endif
}


double ScreenGlobal::getDPI() const {
    return screen.logicalDotsPerInch();
}
