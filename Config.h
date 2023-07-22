//
// Created by qinruihua on 23-7-22.
//

#ifndef KDE_WINE_HELPER_CONFIG_H
#define KDE_WINE_HELPER_CONFIG_H

#include <QString>

struct Config {

    // 需要监听的窗口名称
    QString windowName;

    // 当窗口处于DemandsAttention状态时，是否自动切换到该窗口
    bool autoActivateWindow;

    // 当窗口处于DemandsAttention状态时，是否展示托盘通知
    bool showTrayNotify;

    // 托盘图标
    QString trayIcon;

    // 托盘通知的标题
    QString notifyTitle;

    // 托盘通知的内容
    QString notifyContent;

    // 托盘通知的展示时长
    int showTrayNotifyDurationInSeconds;
};

#endif //KDE_WINE_HELPER_CONFIG_H
