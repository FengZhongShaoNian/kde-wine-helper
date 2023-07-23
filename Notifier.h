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
    explicit Information(const Config& config){
        this->title = config.notifyTitle;
        this->content = config.notifyContent;
        this->millSeconds = config.showTrayNotifyDurationInSeconds * 1000;
    }

    [[nodiscard]] const QString &getTitle() const {
        return title;
    }

    [[nodiscard]] const QString &getContent() const {
        return content;
    }

    [[nodiscard]] int getMillSeconds() const {
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
    explicit Notifier(QIcon trayIcon, QString trayTooltip, QObject *parent = nullptr);

    ~Notifier() override;

public:

    void showInformation(const QString& title, const QString& content, int millSeconds);

    void showInformation(Information& information);

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

    signals:
    /**
     * 用户点击了托盘图标的「退出」菜单
     */
    void menuActionExitClicked();

    /**
     * 托盘图标被点击
     */
    void trayIconClicked();

protected slots:
    void onTimerTimeout();

private:
    QSystemTrayIcon *systemTrayIcon;
    QIcon normalIcon, transparentIcon;
    QMenu* trayMenu;
    QTimer* timer;
    bool blinking = false;
};

#endif //WECHATMESSAGENOTIFIER_NOTIFIER_H
