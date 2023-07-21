//
// Created by qinruihua on 23-7-21.
//

#ifndef WECHATMESSAGENOTIFIER_NOTIFIER_H
#define WECHATMESSAGENOTIFIER_NOTIFIER_H

#include <QObject>
#include <QSystemTrayIcon>

class Notifier : public QObject {
Q_OBJECT

public:
    explicit Notifier(QObject* parent = nullptr);

    ~Notifier() override;

    void showInformation(const QString& title, const QString& content, int millSeconds);

private:
    QSystemTrayIcon *trayIcon;
};

#endif //WECHATMESSAGENOTIFIER_NOTIFIER_H
