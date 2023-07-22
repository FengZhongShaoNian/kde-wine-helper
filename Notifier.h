//
// Created by qinruihua on 23-7-21.
//

#ifndef WECHATMESSAGENOTIFIER_NOTIFIER_H
#define WECHATMESSAGENOTIFIER_NOTIFIER_H

#include "Config.h"

#include <QObject>
#include <QSystemTrayIcon>
#include <QTimer>

class Information {
public:
    Information(Config config){
        this->title = config.notifyTitle;
        this->content = config.notifyContent;
        this->millSeconds = config.showTrayNotifyDurationInSeconds * 1000;
    }

    const QString &getTitle() const {
        return title;
    }

    const QString &getContent() const {
        return content;
    }

    int getMillSeconds() const {
        return millSeconds;
    }

private:
    QString title;
    QString content;
    int millSeconds;
};

class Notifier : public QObject {
Q_OBJECT

public:
    explicit Notifier(QIcon trayIcon, QObject* parent = nullptr);

    ~Notifier() override;

    void showInformation(const QString& title, const QString& content, int millSeconds);

    void showInformation(Information& information);

    signals:
    void exitApp();
public:

    /**
     * 让托盘图标开始闪烁
     */
    void startBlink();

    /**
     * 停止托盘图标闪烁
     */
    void stopBlink();

    /**
     * 隐藏托盘图标
     */
    void hideTrayIcon();

    /**
     * 显示托盘图标
     */
    void showTrayIcon();

protected slots:
    void onTimerTimeout();

private:
    QSystemTrayIcon *systemTrayIcon;
    QIcon normalIcon, transparentIcon;
    QTimer* timer;
    bool blinking = false;
};

#endif //WECHATMESSAGENOTIFIER_NOTIFIER_H
