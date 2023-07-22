//
// Created by qinruihua on 23-7-21.
//

#include "Notifier.h"

#include <QMenu>
#include <QAction>
#include <QApplication>
#include <QDebug>

// Qt D-Bus
#include <QDBusPendingReply>
#include <QDBusInterface>
#include <QDBusPendingCallWatcher>
#include <utility>

Notifier::Notifier(QIcon trayIcon, QObject *parent) : QObject(parent) {
    this->systemTrayIcon = new QSystemTrayIcon(parent);

    this->normalIcon = std::move(trayIcon);
    this->transparentIcon=QIcon("./icons/transparent.png");

    systemTrayIcon->setIcon(this->normalIcon);
    systemTrayIcon->show();

    this->timer = new QTimer(this);
    timer->setInterval(1000);
    QObject::connect(timer, &QTimer::timeout, this, &Notifier::onTimerTimeout);
}

Notifier::~Notifier() noexcept {
    if(systemTrayIcon != nullptr){
        delete systemTrayIcon;
        systemTrayIcon = nullptr;
    }
}

void Notifier::showInformation(const QString& title, const QString& content, int millSeconds) {
    this->systemTrayIcon->showMessage(title, content, QSystemTrayIcon::Information, millSeconds);
}

void Notifier::showInformation(Information &information) {
    this->showInformation(information.getTitle(), information.getContent(), information.getMillSeconds());
}

void Notifier::startBlink() {
    if(!this->systemTrayIcon->isVisible()){
        this->systemTrayIcon->setVisible(true);
    }
    if(this->blinking){
        return;
    }
    timer->start();
    this->blinking = true;
}

void Notifier::stopBlink() {
    if(this->blinking){
        timer->stop();
        this->blinking = false;
        this->systemTrayIcon->setIcon(this->normalIcon);
    }
}

void Notifier::onTimerTimeout() {
    QIcon currentIcon = this->systemTrayIcon->icon();
    if(currentIcon.cacheKey() == this->normalIcon.cacheKey()){
        this->systemTrayIcon->setIcon(this->transparentIcon);
    } else{
        this->systemTrayIcon->setIcon(this->normalIcon);
    }
}

void Notifier::hideTrayIcon() {
    if(this->blinking){
        stopBlink();
    }
    if(this->systemTrayIcon->isVisible()){
        this->systemTrayIcon->setVisible(false);
    }
}

void Notifier::showTrayIcon() {
    if(!this->systemTrayIcon->isVisible()){
        this->systemTrayIcon->setVisible(true);
    }
}

