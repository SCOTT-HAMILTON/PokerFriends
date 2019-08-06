#include "size.h"
#include <QDebug>
#include <QGuiApplication>
#include <QScreen>

int Size::APP_SIZEW = 360;
int Size::APP_SIZEH = 720;

#ifdef ANDROID_SIZE
    double Size::SIZE_FACTOR = 1;
#else
    double Size::SIZE_FACTOR = 0.5;
#endif

int Size::APP_X = 0;
int Size::APP_Y = 0;

void Size::updateSize()
{
    qDebug() << QGuiApplication::screens().first()->availableSize();
    {
        int w = QGuiApplication::screens().first()->availableSize().width();
        int h = QGuiApplication::screens().first()->availableSize().height();
#ifdef Q_OS_ANDROID
        //LANDSCAPE VIEW or less than 18:9 (like 16:9)
        if (w>=h/2){
            Size::APP_SIZEH = h;
            Size::APP_SIZEW = static_cast<int>(static_cast<double>(h)/static_cast<double>(1.81944444444444444444));
            Size::APP_X = (w-Size::APP_SIZEW)/2;
        }
        //for even more little ratio than 18:9 like 20:9
        else {
            Size::APP_SIZEW = w;
            Size::APP_SIZEH = w*2;
            Size::APP_Y = (h-Size::APP_SIZEH)/2;
        }
        Size::SIZE_FACTOR = static_cast<double>(Size::APP_SIZEH)/720.0;

#else
        h -= 60;
        Size::APP_SIZEH = h;
        Size::APP_SIZEW = h*720/1310;
        Size::SIZE_FACTOR = static_cast<double>(Size::APP_SIZEH)/655.0;

#endif

    }
    qDebug() << "SIZE_FACTOR : " << Size::SIZE_FACTOR;
    qDebug() << "APP SIZE : " << Size::APP_SIZEW << ", " << Size::APP_SIZEH;
//    Size::APP_SIZEW = 720;
//    Size::APP_SIZEH = 1310;
//    Size::APP_X = 0;
//    Size::SIZE_FACTOR = 2;
//    qDebug() << "SIZE_FACTOR : " << Size::SIZE_FACTOR;
//    qDebug() << "APP SIZE : " << Size::APP_SIZEW << ", " << Size::APP_SIZEH;
//
}
