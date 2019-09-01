#include <QApplication>
#include <QQuickView>
#include <QQmlContext>
#include <QQmlEngine>
#include "ui/widget.h"
#include "ui/size.h"

#include <QtCore/QSettings>
#include <QtNetwork/QNetworkConfigurationManager>
#include <QtNetwork/QNetworkSession>

#include <QOpenGLContext>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    qputenv("QT_QUICK_CONTROLS_STYLE", "material");

    QSurfaceFormat format;
    if (QOpenGLContext::openGLModuleType() == QOpenGLContext::LibGL) { // Learn OpenGL
        format.setRenderableType(QSurfaceFormat::OpenGL);
        format.setVersion(3, 3);
        format.setProfile(QSurfaceFormat::CoreProfile);
        format.setSamples(4);
        qDebug() << "OPENGL version : " << format.version();
    } else if (QOpenGLContext::openGLModuleType() == QOpenGLContext::LibGLES) { // Learn OpenGLES??
        format.setRenderableType(QSurfaceFormat::OpenGLES);
        format.setVersion(3, 1);
        format.setSamples(0);
        qDebug() << "OPENGLES version : " << format.version();
    }
//    format.setAlphaBufferSize(0);
//    format.setDepthBufferSize(0);
    format.setStencilBufferSize(0);
//    format.setSwapInterval(0); // Full speed rendering
    QSurfaceFormat::setDefaultFormat(format);

    Size::updateSize();

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
