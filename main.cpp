#include "XWindowListener.h"

#include <QApplication>
#include <QDebug>
#include <QtX11Extras/QX11Info>
#include <QtGui/QWindow>
#include <QSystemTrayIcon>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

#include <KWindowSystem>
#include <KWindowInfo>
#include <QJsonArray>

QList<Config> readConfig()
{
    QList<Config> configs;

    QFile file("./config.json");
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open file './config.json' .");
        return configs;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument doc(QJsonDocument::fromJson(jsonData));
    QJsonArray jsonArray = doc.array();

    for(int i = 0; i < jsonArray.count(); i++){

        QJsonValue value = jsonArray.at(i);
        assert(value.isObject());

        QJsonObject jsonObject = value.toObject();

        QString windowName = jsonObject["windowName"].toString();
        bool autoActivateWindow = jsonObject["autoActivateWindow"].toBool();
        bool showTrayNotify = jsonObject["showTrayNotify"].toBool();
        QString notifyTitle = jsonObject["notifyTitle"].toString();
        QString notifyContent = jsonObject["notifyContent"].toString();
        int showTrayNotifyDurationInSeconds = jsonObject["showTrayNotifyDurationInSeconds"].toInt();

        qDebug() << "windowName: " << windowName
        << "\nautoActivateWindow: " << autoActivateWindow
        << "\nshowTrayNotify: " << showTrayNotify
        << "\nnotifyTitle: " << notifyTitle
        << "\nnotifyContent: " << notifyContent
        << "\nshowTrayNotifyDurationInSeconds: " << showTrayNotifyDurationInSeconds
        << Qt::endl;

        Config config {
            windowName = windowName,
            autoActivateWindow = autoActivateWindow,
            showTrayNotify = showTrayNotify,
                notifyTitle = notifyTitle,
                notifyContent = notifyContent,
                showTrayNotifyDurationInSeconds = showTrayNotifyDurationInSeconds
        };
        configs.append(config);
    }
    return configs;
}


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QList<Config> configs = readConfig();
    Notifier notifier(&app);

    XWindowListener xWindowListener(configs, &notifier, &app);
    QObject::connect(KWindowSystem::self()
            , static_cast<void (KWindowSystem::*)(WId, NET::Properties, NET::Properties2)>
                     (&KWindowSystem::windowChanged)
            , &xWindowListener, &XWindowListener::onWindowChanged);

    return QApplication::exec();
}