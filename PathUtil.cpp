//
// Created by qinruihua on 23-7-23.
//
#include "PathUtil.h"

#include <QCoreApplication>
#include <QDir>

QString resolveFile(QString relativePath){
    if(relativePath.startsWith("./")){
        QString appDir = QCoreApplication::applicationDirPath();
        QDir dir(appDir);
        QString filePath = dir.filePath(relativePath);
        return filePath;
    }
    return relativePath;
}