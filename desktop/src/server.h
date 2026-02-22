// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 Multi-Function PC Remote Contributors

#pragma once

#include <QObject>
#include <QWebSocketServer>
#include <QWebSocket>
#include <QList>
#include <memory>

class MediaController;
class InputController;
class FileTransfer;
class SystemController;
class ScreenShare;

class Server : public QObject
{
    Q_OBJECT

public:
    explicit Server(QObject *parent = nullptr);
    ~Server();

    bool start(quint16 port);
    void stop();

private slots:
    void onNewConnection();
    void onTextMessageReceived(const QString &message);
    void onSocketDisconnected();

private:
    void handleCommand(QWebSocket *client, const QJsonObject &command);

    QWebSocketServer *m_server;
    QList<QWebSocket *> m_clients;
    
    std::unique_ptr<MediaController> m_mediaController;
    std::unique_ptr<InputController> m_inputController;
    std::unique_ptr<FileTransfer> m_fileTransfer;
    std::unique_ptr<SystemController> m_systemController;
    std::unique_ptr<ScreenShare> m_screenShare;
};
