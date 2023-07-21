//
// Created by qinruihua on 23-7-21.
//
#include "XWindowListener.h"
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <utility>

XWindowListener::XWindowListener(QList<Config> configs,
                                 Notifier *notifier,
                                 QObject *parent) : QObject(parent) {
    this->configs = std::move(configs);
    this->notifier = notifier;
}

XWindowListener::~XWindowListener() {
}


void XWindowListener::onWindowChanged(WId wid, NET::Properties prop1, NET::Properties2 prop2) {
    qDebug() << "windowChange(WId wid, NET::Properties prop1, NET::Properties2 prop2) invoked" << Qt::endl;
    if (prop1 & NET::WMState) {

        KWindowInfo nameInfo(wid, NET::WMName);
        const QString windowName = nameInfo.name();

        qDebug() << "Window [ wid=" << wid << ", name = " << windowName << "] NET::WMState changed" << Qt::endl;

        KWindowInfo stateInfo(wid, NET::WMState);
        const bool demandsAttention = stateInfo.hasState(NET::DemandsAttention);
        if (demandsAttention) {
            qDebug() << "Window [ wid=" << wid << ", name = " << windowName << "] demands Attention" << Qt::endl;

            for (auto config : configs) {
                if(config.windowName == windowName){
                    if(config.autoActivateWindow){
                        KWindowSystem::activateWindow(wid);
                    }
                    if(config.showTrayNotify){
                        this->notifier->showInformation(config.notifyTitle,
                                                        config.notifyContent,
                                                        config.showTrayNotifyDurationInSeconds * 1000);
                    }
                }
            }
        }
    }
}
