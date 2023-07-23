//
// Created by qinruihua on 23-7-21.
//
#include "XWindowListener.h"
#include "Notifier.h"
#include "PathUtil.h"

#include <stdexcept>

#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <KWindowInfo>


XWindowListener::XWindowListener(const QList<Config>& configs,
                                 QObject *parent) : QObject(parent) {
    this->configMap = QMap<QString, Config>();
    this->notifierMap = QMap<QString, Notifier*>();

    for (auto &config: configs){
        this->configMap[config.windowName] = config;

        QIcon icon(resolveFile(config.trayIcon));
        auto notifier = new Notifier(icon, this);
        this->notifierMap[config.windowName] = notifier;
    }

    showTrayIconIfInterestWindowOpened();

    this->timer = new QTimer(this);
    this->timer->setInterval(1000*5);
    QObject::connect(this->timer, &QTimer::timeout, this, &XWindowListener::onTimeout);
    this->timer->start();

    QObject::connect(KX11Extras::self()
            , static_cast<void (KX11Extras::*)(WId, NET::Properties, NET::Properties2)>
                     (&KX11Extras::windowChanged)
            , this, &XWindowListener::onWindowChanged);

    QObject::connect(KX11Extras::self(),
                     static_cast<void (KX11Extras::*)(WId)>
                     (&KX11Extras::windowRemoved),
             this, &XWindowListener::onWindowRemoved);

    QObject::connect(KX11Extras::self(),
                     static_cast<void (KX11Extras::*)(WId)>
                     (&KX11Extras::activeWindowChanged),
                     this, &XWindowListener::onActiveWindowChanged);
}

XWindowListener::~XWindowListener() = default;

void XWindowListener::showTrayIconIfInterestWindowOpened() {
    QSet<WId> windowIdSet = listAllInterestWindowsThatHaveBeenOpened();
    QSet<QString> windowNameSet;
    for (const auto &windowId: windowIdSet){
        windowNameSet.insert(getWindowName(windowId));
    }

    for (auto it = notifierMap.constBegin(); it != notifierMap.constEnd(); ++it) {
        const QString &key = it.key();
        auto notifier = it.value();
        if(windowNameSet.contains(key)){
            notifier->showTrayIcon();
        }
    }
}

void XWindowListener::onWindowChanged(WId wid, NET::Properties prop1, NET::Properties2 prop2) {
    qDebug() << "windowChange(WId wid, NET::Properties prop1, NET::Properties2 prop2) invoked" << Qt::endl;
    if (prop1 & NET::WMState) {

        QString windowName = getWindowName(wid);

        if(!this->configMap.contains(windowName)){
            return;
        }

        qDebug() << "Window [ wid=" << wid << ", name = " << windowName << "] NET::WMState changed" << Qt::endl;

        const bool windowDemandsAttention = isWindowDemandsAttention(wid);
        if (windowDemandsAttention) {
            qDebug() << "Window [ wid=" << wid << ", name = " << windowName << "] demands Attention" << Qt::endl;
        }

        Config& config = getConfigForWindow(windowName);
        Notifier *notifier = getNotifierFor(windowName);

        if(windowDemandsAttention){
            notifier->startBlink();

            if(config.autoActivateWindow){
                KX11Extras::activateWindow(wid);
            }

            if(config.showTrayNotify){
                Information info(config);
                notifier->showInformation(info);
            }
        } else{
            notifier->stopBlink();
        }

    }
}

Notifier* XWindowListener::getNotifierFor(QString &windowName) {
    Notifier* notifier= notifierMap[windowName];
    assert(notifier != nullptr);
    return notifier;
}

void XWindowListener::onTimeout() {
    QSet<WId> windowIdList = listAllInterestWindowsThatHaveBeenOpened();
    for (const auto &item: windowIdList){
        QString windowName = getWindowName(item);
        Notifier* notifier = getNotifierFor(windowName);
        if(isWindowDemandsAttention(item)){
            notifier->startBlink();
        } else {
            notifier->stopBlink();
        }
    }
}

void XWindowListener::onWindowRemoved(WId wid) {
     QString windowName = getWindowName(wid);

    if(isInterestWindow(wid)){
        qDebug() << "Window [ wid=" << wid << ", name = " << windowName << "] closed" << Qt::endl;

        auto notifier = getNotifierFor(windowName);
        notifier->hideTrayIcon();
    }
}

QSet<WId> XWindowListener::listAllInterestWindowsThatHaveBeenOpened() {
    QSet<WId> interestWindows;
    QList<WId> windowIdList = KX11Extras::windows();
    for (const auto &wid: windowIdList){
        if(isInterestWindow(wid)){
            interestWindows.insert(wid);
        }
    }
    return interestWindows;
}

bool XWindowListener::isInterestWindow(WId wid) {
    QString windowName = getWindowName(wid);
    return this->configMap.contains(windowName);
}

bool XWindowListener::isWindowDemandsAttention(WId wid) {
    KWindowInfo stateInfo(wid, NET::WMState);
    return stateInfo.hasState(NET::DemandsAttention);
}


QString XWindowListener::getWindowName(WId wid) {
    KWindowInfo nameInfo(wid, NET::WMName);
    return nameInfo.name();
}

Config& XWindowListener::getConfigForWindow(QString& windowName) {
    if(this->configMap.contains(windowName)){
        return configMap[windowName];
    }
    throw std::runtime_error("No config found for window: " + windowName.toStdString());
}

void XWindowListener::onActiveWindowChanged(WId wid) {
    qDebug() << "window [wid=" << wid << "] active" << Qt::endl;
    WId  currentActiveWindow = wid;
    if(!isInterestWindow(currentActiveWindow)){
        return;
    }

    QString windowName = getWindowName(currentActiveWindow);
    qDebug() << "show tray icon, since window [name=" << windowName << "] active";

    auto notifier = getNotifierFor(windowName);
    notifier->showTrayIcon();
}










