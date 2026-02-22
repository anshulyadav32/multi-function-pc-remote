// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 Multi-Function PC Remote Contributors

#pragma once

#include <QObject>
#include <QJsonObject>

class MediaController : public QObject
{
    Q_OBJECT

public:
    explicit MediaController(QObject *parent = nullptr);
    
    void handleAction(const QString &action, const QJsonObject &data);

private:
    void playPause();
    void nextTrack();
    void previousTrack();
    void setVolume(int volume);
    void mute();
};
