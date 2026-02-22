// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 Multi-Function PC Remote Contributors

#include "systemcontroller.h"
#include <QProcess>
#include <QDebug>

SystemController::SystemController(QObject *parent)
    : QObject(parent)
{
}

void SystemController::handleAction(const QString &action)
{
    if (action == "shutdown") {
        shutdown();
    } else if (action == "restart") {
        restart();
    } else if (action == "sleep") {
        sleep();
    } else if (action == "lock") {
        lock();
    }
}

void SystemController::shutdown()
{
#ifdef Q_OS_WIN
    QProcess::startDetached("shutdown", QStringList() << "/s" << "/t" << "0");
#elif defined(Q_OS_LINUX)
    QProcess::startDetached("shutdown", QStringList() << "-h" << "now");
#elif defined(Q_OS_MAC)
    QProcess::startDetached("shutdown", QStringList() << "-h" << "now");
#endif
    qDebug() << "System: Shutdown";
}

void SystemController::restart()
{
#ifdef Q_OS_WIN
    QProcess::startDetached("shutdown", QStringList() << "/r" << "/t" << "0");
#elif defined(Q_OS_LINUX)
    QProcess::startDetached("shutdown", QStringList() << "-r" << "now");
#elif defined(Q_OS_MAC)
    QProcess::startDetached("shutdown", QStringList() << "-r" << "now");
#endif
    qDebug() << "System: Restart";
}

void SystemController::sleep()
{
#ifdef Q_OS_WIN
    QProcess::startDetached("rundll32.exe", QStringList() << "powrprof.dll,SetSuspendState" << "0,1,0");
#elif defined(Q_OS_LINUX)
    QProcess::startDetached("systemctl", QStringList() << "suspend");
#elif defined(Q_OS_MAC)
    QProcess::startDetached("pmset", QStringList() << "sleepnow");
#endif
    qDebug() << "System: Sleep";
}

void SystemController::lock()
{
#ifdef Q_OS_WIN
    QProcess::startDetached("rundll32.exe", QStringList() << "user32.dll,LockWorkStation");
#elif defined(Q_OS_LINUX)
    QProcess::startDetached("loginctl", QStringList() << "lock-session");
#elif defined(Q_OS_MAC)
    QProcess::startDetached("/System/Library/CoreServices/Menu Extras/User.menu/Contents/Resources/CGSession", QStringList() << "-suspend");
#endif
    qDebug() << "System: Lock";
}
