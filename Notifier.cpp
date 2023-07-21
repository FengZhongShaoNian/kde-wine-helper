//
// Created by qinruihua on 23-7-21.
//

#include <QMenu>
#include <QAction>
#include <QApplication>
#include "Notifier.h"

Notifier::Notifier(QObject *parent) : QObject(parent) {
    this->trayIcon = new QSystemTrayIcon(parent);
    trayIcon->setIcon(QIcon("./icons/app.png"));

    auto menu = new QMenu();
    auto quit = new QAction("退出");
    connect(quit, &QAction::triggered, [&](){
        QApplication::quit();
    });

    menu->addAction(quit);

    trayIcon->setContextMenu(menu);
    trayIcon->show();
}

Notifier::~Notifier() noexcept {
    if(trayIcon != nullptr){
        delete trayIcon;
        trayIcon = nullptr;
    }
}

void Notifier::showInformation(const QString& title, const QString& content, int millSeconds) {
    this->trayIcon->showMessage(title, content, QSystemTrayIcon::Information, millSeconds);
}
