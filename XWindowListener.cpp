//
// Created by qinruihua on 23-7-21.
//
#include "XWindowListener.h"
#include "Notifier.h"

#include <stdexcept>

#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

XWindowListener::XWindowListener(const QList<Config>& configs,
                                 QObject *parent) : QObject(parent) {
    this->configMap = QMap<QString, Config>();
    this->notifierMap = QMap<QString, Notifier*>();

    for (auto &config: configs){
        this->configMap[config.windowName] = config;

        QIcon icon(config.trayIcon);
        auto notifier = new Notifier(icon, this);
        this->notifierMap[config.windowName] = notifier;
    }

    QSet<WId> windowIdSet = listAllInterestWindowsId();
    QSet<QString> windowNameSet;
    for (const auto &windowId: windowIdSet){
        windowNameSet.insert(getWindowName(windowId));
    }

    for (auto it = this->notifierMap.constBegin(); it != this->notifierMap.constEnd(); ++it) {
        const QString &key = it.key();
        auto notifier = it.value();
        if(!windowNameSet.contains(key)){
            notifier->hideTrayIcon();
        }
    }

    this->timer = new QTimer(this);
    this->timer->setInterval(1000*5);
    QObject::connect(this->timer, &QTimer::timeout, this, &XWindowListener::onTimeout);
    this->timer->start();

    QObject::connect(KWindowSystem::self()
            , static_cast<void (KWindowSystem::*)(WId, NET::Properties, NET::Properties2)>
                     (&KWindowSystem::windowChanged)
            , this, &XWindowListener::onWindowChanged);

    QObject::connect(KWindowSystem::self(),
                     static_cast<void (KWindowSystem::*)(WId)>
                     (&KWindowSystem::windowRemoved),
             this, &XWindowListener::onWindowRemoved);

    QObject::connect(KWindowSystem::self(),
                     static_cast<void (KWindowSystem::*)(WId)>
                     (&KWindowSystem::activeWindowChanged),
                     this, &XWindowListener::onActiveWindowChanged);
}

XWindowListener::~XWindowListener() = default;


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

        Config& config = this->configMap[windowName];
        Notifier *notifier = getNotifierFor(windowName);

        if(windowDemandsAttention){
            notifier->startBlink();

            if(config.autoActivateWindow){
                KWindowSystem::activateWindow(wid);
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
    QSet<WId> windowIdList = listAllInterestWindowsId();
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

QSet<WId> XWindowListener::listAllInterestWindowsId() {
    QSet<WId> interestWindows;
    QList<WId> windowIdList = KWindowSystem::windows();
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










