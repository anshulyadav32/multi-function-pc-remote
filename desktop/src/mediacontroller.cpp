// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 Multi-Function PC Remote Contributors

#include "mediacontroller.h"
#include <QDebug>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

MediaController::MediaController(QObject *parent)
    : QObject(parent)
{
}

void MediaController::handleAction(const QString &action, const QJsonObject &data)
{
    if (action == "play_pause") {
        playPause();
    } else if (action == "next") {
        nextTrack();
    } else if (action == "previous") {
        previousTrack();
    } else if (action == "volume") {
        int volume = data["value"].toInt();
        setVolume(volume);
    } else if (action == "mute") {
        mute();
    }
}

void MediaController::playPause()
{
#ifdef Q_OS_WIN
    keybd_event(VK_MEDIA_PLAY_PAUSE, 0, 0, 0);
    keybd_event(VK_MEDIA_PLAY_PAUSE, 0, KEYEVENTF_KEYUP, 0);
#endif
    qDebug() << "Media: Play/Pause";
}

void MediaController::nextTrack()
{
#ifdef Q_OS_WIN
    keybd_event(VK_MEDIA_NEXT_TRACK, 0, 0, 0);
    keybd_event(VK_MEDIA_NEXT_TRACK, 0, KEYEVENTF_KEYUP, 0);
#endif
    qDebug() << "Media: Next Track";
}

void MediaController::previousTrack()
{
#ifdef Q_OS_WIN
    keybd_event(VK_MEDIA_PREV_TRACK, 0, 0, 0);
    keybd_event(VK_MEDIA_PREV_TRACK, 0, KEYEVENTF_KEYUP, 0);
#endif
    qDebug() << "Media: Previous Track";
}

void MediaController::setVolume(int volume)
{
    // Volume control implementation
    qDebug() << "Media: Set Volume to" << volume;
}

void MediaController::mute()
{
#ifdef Q_OS_WIN
    keybd_event(VK_VOLUME_MUTE, 0, 0, 0);
    keybd_event(VK_VOLUME_MUTE, 0, KEYEVENTF_KEYUP, 0);
#endif
    qDebug() << "Media: Mute Toggle";
}
