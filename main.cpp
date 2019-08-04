#include <QApplication>
#include <QScreen>

#include "widget.h"
#include "size.h"

#include <QtCore/QSettings>
#include <QtNetwork/QNetworkConfigurationManager>
#include <QtNetwork/QNetworkSession>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qDebug() << QGuiApplication::screens().first()->availableSize();
        {
            int w = QGuiApplication::screens().first()->availableSize().width();
            int h = QGuiApplication::screens().first()->availableSize().height();
    #ifdef Q_OS_ANDROID
            //LANDSCAPE VIEW or less than 18:9 (like 16:9)
            if (w>=h/2){
                Size::APP_SIZEH = h;
                Size::APP_SIZEW = h/2;
                Size::APP_X = (w-Size::APP_SIZEW)/2;
            }
            //for even more little ratio than 18:9 like 20:9
            else {
                Size::APP_SIZEW = w;
                Size::APP_SIZEH = w*2;
                Size::APP_Y = (h-Size::APP_SIZEH)/2;
            }
            Size::SIZE_FACTOR = static_cast<double>(Size::APP_SIZEH)/655.0;

    #else
            h -= 80;
            Size::APP_SIZEH = h;
            Size::APP_SIZEW = h*960/1310;
            Size::SIZE_FACTOR = static_cast<double>(Size::APP_SIZEH)/655.0;

    #endif

        }

        qDebug() << "SIZE_FACTOR : " << Size::SIZE_FACTOR;
        qDebug() << "APP SIZE : " << Size::APP_SIZEW << ", " << Size::APP_SIZEH;

    QNetworkConfigurationManager manager;
    if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {
        // Get saved network configuration
        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
        settings.endGroup();

        // If the saved network configuration is not currently discovered use the system default
        QNetworkConfiguration config = manager.configurationFromIdentifier(id);
        if ((config.state() & QNetworkConfiguration::Discovered) !=
            QNetworkConfiguration::Discovered) {
            config = manager.defaultConfiguration();
        }

        QNetworkSession *networkSession = new QNetworkSession(config, &app);
        networkSession->open();
        networkSession->waitForOpened();

        if (networkSession->isOpen()) {
            // Save the used configuration
            QNetworkConfiguration config = networkSession->configuration();
            QString id;
            if (config.type() == QNetworkConfiguration::UserChoice) {
                id = networkSession->sessionProperty(
                        QLatin1String("UserChoiceConfiguration")).toString();
            } else {
                id = config.identifier();
            }

            QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
            settings.beginGroup(QLatin1String("QtNetwork"));
            settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
            settings.endGroup();
        }
    }

    Widget widget;
    widget.show();
    return app.exec();
}
