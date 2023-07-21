//
// Created by qinruihua on 23-7-21.
//

#ifndef WECHATMESSAGENOTIFIER_XWINDOWLISTENER_H
#define WECHATMESSAGENOTIFIER_XWINDOWLISTENER_H

#include "Notifier.h"
#include <KWindowSystem>

struct Config {

    // 需要监听的窗口名称
    QString windowName;

    // 当窗口处于DemandsAttention状态时，是否自动切换到该窗口
    bool autoActivateWindow;

    // 当窗口处于DemandsAttention状态时，是否展示托盘通知
    bool showTrayNotify;

    // 托盘通知的标题
    QString notifyTitle;

    // 托盘通知的内容
    QString notifyContent;

    // 托盘通知的展示时长
    int showTrayNotifyDurationInSeconds;
};

class XWindowListener : public QObject{
    Q_OBJECT
public:
    explicit XWindowListener(QList<Config> configs,
                             Notifier *notifier,
                             QObject *parent = nullptr);
    virtual ~XWindowListener();

    void onWindowChanged(WId wid, NET::Properties prop1, NET::Properties2 prop2);

private:
    QList<Config> configs;
    Notifier* notifier;
};

#endif //WECHATMESSAGENOTIFIER_XWINDOWLISTENER_H