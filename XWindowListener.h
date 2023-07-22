//
// Created by qinruihua on 23-7-21.
//

#ifndef WECHATMESSAGENOTIFIER_XWINDOWLISTENER_H
#define WECHATMESSAGENOTIFIER_XWINDOWLISTENER_H

#include "Notifier.h"
#include "Config.h"

#include <QMap>
#include <KWindowSystem>

class XWindowListener : public QObject{
    Q_OBJECT
public:
    explicit XWindowListener(const QList<Config>& configs,
                             QObject *parent = nullptr);
    virtual ~XWindowListener();

public slots:
    void onWindowChanged(WId wid, NET::Properties prop1, NET::Properties2 prop2);

    void onTimeout();

    void onWindowRemoved(WId wid);

    void onActiveWindowChanged(WId wid);

public:
    /**
     * @return 所有被用户感兴趣的窗口
     */
    QSet<WId> listAllInterestWindowsId();

    /**
     * 判断指定窗口是否是用户感兴趣的窗口
     * @param wid 窗口id
     * @return 窗口是否是用户感兴趣的窗口
     */
    bool isInterestWindow(WId wid);

    /**
     * 判断窗口是否需要关注
     * @param wid 窗口id
     * @return 窗口是否处于需要关注的状态
     */
    static bool isWindowDemandsAttention(WId wid);

    static QString getWindowName(WId wid);

private:
    QMap<QString, Config> configMap;
    QMap<QString, Notifier*> notifierMap;
    QTimer* timer;

    Config& getConfigForWindow(QString& windowName);
    Notifier* getNotifierFor(QString &windowName);
};

#endif //WECHATMESSAGENOTIFIER_XWINDOWLISTENER_H