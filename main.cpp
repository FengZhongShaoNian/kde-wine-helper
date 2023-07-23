#include "XWindowListener.h"
#include "PathUtil.h"

#include <QApplication>
#include <QDebug>
#include <QtX11Extras/QX11Info>
#include <QtGui/QWindow>
#include <QSystemTrayIcon>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include <KWindowInfo>
#include <QMenu>

QList<Config> readConfig()
{
    QList<Config> configs;

    QFile file(resolveFile("./config.json"));
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
        QString trayIcon = jsonObject["trayIcon"].toString();
        QString notifyContent = jsonObject["notifyContent"].toString();
        int showTrayNotifyDurationInSeconds = jsonObject["showTrayNotifyDurationInSeconds"].toInt();

        qDebug() << "windowName: " << windowName
        << "\nautoActivateWindow: " << autoActivateWindow
        << "\nshowTrayNotify: " << showTrayNotify
        << "\ntrayIcon: " << trayIcon
        << "\nnotifyTitle: " << notifyTitle
        << "\nnotifyContent: " << notifyContent
        << "\nshowTrayNotifyDurationInSeconds: " << showTrayNotifyDurationInSeconds
        << Qt::endl;

        Config config {
            windowName = windowName,
            autoActivateWindow = autoActivateWindow,
            showTrayNotify = showTrayNotify,
                trayIcon = trayIcon,
                notifyTitle = notifyTitle,
                notifyContent = notifyContent,
                showTrayNotifyDurationInSeconds = showTrayNotifyDurationInSeconds
        };
        configs.append(config);
    }
    return configs;
}

void createAppTrayIcon(QApplication* app){
    auto systemTrayIcon = new QSystemTrayIcon(app);

    QIcon appIcon(resolveFile("./icons/app.png"));

    systemTrayIcon->setIcon(appIcon);

    auto menu = new QMenu();
    auto quit = new QAction("退出");
    QObject::connect(quit, &QAction::triggered, [&](){
        QApplication::quit();
    });

    menu->addAction(quit);

    systemTrayIcon->setContextMenu(menu);
    systemTrayIcon->show();
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QList<Config> configs = readConfig();

    XWindowListener xWindowListener(configs, &app);

    createAppTrayIcon(&app);
    return QApplication::exec();
}