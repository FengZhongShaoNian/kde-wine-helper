//
// Created by qinruihua on 23-7-21.
//

#include "Notifier.h"

#include <utility>

#include <QMenu>
#include <QAction>
#include <QApplication>
#include <QDebug>

Notifier::Notifier(QIcon trayIcon, QString trayTooltip, QObject *parent) : QObject(parent) {
    this->systemTrayIcon = new QSystemTrayIcon(parent);

    this->normalIcon = std::move(trayIcon);
    this->transparentIcon=QIcon("./icons/transparent.png");

    systemTrayIcon->setIcon(this->normalIcon);
    systemTrayIcon->setToolTip(trayTooltip);

    this->trayMenu = new QMenu();
    auto actionExit = new QAction("退出",this);
    QObject::connect(actionExit, &QAction::triggered, [this](){
       emit menuActionExitClicked();
    });
    this->trayMenu->addAction(actionExit);
    systemTrayIcon->setContextMenu(this->trayMenu);

    QObject::connect(systemTrayIcon, &QSystemTrayIcon::activated, [this](QSystemTrayIcon::ActivationReason reason){
        if(reason == QSystemTrayIcon::ActivationReason::Trigger){
            emit this->trayIconClicked();
        }
    });

    this->timer = new QTimer(this);
    timer->setInterval(1000);
    QObject::connect(timer, &QTimer::timeout, this, &Notifier::onTimerTimeout);
}

Notifier::~Notifier() noexcept {
    if(this->trayMenu != nullptr){
        delete trayMenu;
        trayMenu = nullptr;
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

