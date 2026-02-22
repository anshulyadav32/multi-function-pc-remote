// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 Multi-Function PC Remote Contributors

#include "screenshare.h"
#include <QScreen>
#include <QGuiApplication>
#include <QPixmap>
#include <QBuffer>
#include <QJsonDocument>
#include <QWebSocket>
#include <QDebug>

ScreenShare::ScreenShare(QObject *parent)
    : QObject(parent)
    , m_captureTimer(new QTimer(this))
{
    connect(m_captureTimer, &QTimer::timeout, this, &ScreenShare::captureAndSendFrame);
}

void ScreenShare::handleRequest(const QJsonObject &request, QWebSocket *client)
{
    QString action = request["action"].toString();
    
    if (action == "start") {
        startStreaming(client);
    } else if (action == "stop") {
        stopStreaming();
    }
}

void ScreenShare::startStreaming(QWebSocket *client)
{
    if (m_streamingClient) {
        stopStreaming();
    }
    
    m_streamingClient = client;
    m_captureTimer->start(100); // 10 FPS
    
    QJsonObject response;
    response["type"] = "screen";
    response["status"] = "streaming";
    client->sendTextMessage(QJsonDocument(response).toJson());
    
    qDebug() << "Screen sharing started";
}

void ScreenShare::stopStreaming()
{
    m_captureTimer->stop();
    m_streamingClient = nullptr;
    qDebug() << "Screen sharing stopped";
}

void ScreenShare::captureAndSendFrame()
{
    if (!m_streamingClient) {
        return;
    }
    
    QScreen *screen = QGuiApplication::primaryScreen();
    if (!screen) {
        return;
    }
    
    QPixmap screenshot = screen->grabWindow(0);
    screenshot = screenshot.scaled(1280, 720, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    screenshot.save(&buffer, "JPEG", 75);
    
    QJsonObject frame;
    frame["type"] = "screen";
    frame["action"] = "frame";
    frame["data"] = QString::fromLatin1(byteArray.toBase64());
    
    m_streamingClient->sendTextMessage(QJsonDocument(frame).toJson());
}
