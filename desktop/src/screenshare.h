// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 Multi-Function PC Remote Contributors

#pragma once

#include <QObject>
#include <QJsonObject>
#include <QTimer>

class QWebSocket;

class ScreenShare : public QObject
{
    Q_OBJECT

public:
    explicit ScreenShare(QObject *parent = nullptr);
    
    void handleRequest(const QJsonObject &request, QWebSocket *client);

private slots:
    void captureAndSendFrame();

private:
    void startStreaming(QWebSocket *client);
    void stopStreaming();
    
    QWebSocket *m_streamingClient = nullptr;
    QTimer *m_captureTimer;
};
