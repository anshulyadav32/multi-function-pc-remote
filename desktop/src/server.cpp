// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 Multi-Function PC Remote Contributors

#include "server.h"
#include "mediacontroller.h"
#include "inputcontroller.h"
#include "filetransfer.h"
#include "systemcontroller.h"
#include "screenshare.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

Server::Server(QObject *parent)
    : QObject(parent)
    , m_server(new QWebSocketServer("PC Remote Server", 
                                    QWebSocketServer::NonSecureMode, this))
    , m_mediaController(std::make_unique<MediaController>())
    , m_inputController(std::make_unique<InputController>())
    , m_fileTransfer(std::make_unique<FileTransfer>())
    , m_systemController(std::make_unique<SystemController>())
    , m_screenShare(std::make_unique<ScreenShare>())
{
}

Server::~Server()
{
    stop();
}

bool Server::start(quint16 port)
{
    if (m_server->listen(QHostAddress::Any, port)) {
        connect(m_server, &QWebSocketServer::newConnection,
                this, &Server::onNewConnection);
        qDebug() << "Server started on port" << port;
        return true;
    }
    
    qWarning() << "Failed to start server:" << m_server->errorString();
    return false;
}

void Server::stop()
{
    for (QWebSocket *client : m_clients) {
        client->close();
    }
    m_clients.clear();
    m_server->close();
}

void Server::onNewConnection()
{
    QWebSocket *socket = m_server->nextPendingConnection();
    
    connect(socket, &QWebSocket::textMessageReceived,
            this, &Server::onTextMessageReceived);
    connect(socket, &QWebSocket::disconnected,
            this, &Server::onSocketDisconnected);
    
    m_clients.append(socket);
    qDebug() << "New client connected:" << socket->peerAddress().toString();
    
    // Send welcome message
    QJsonObject response;
    response["type"] = "welcome";
    response["version"] = "1.0.0";
    socket->sendTextMessage(QJsonDocument(response).toJson(QJsonDocument::Compact));
}

void Server::onTextMessageReceived(const QString &message)
{
    QWebSocket *client = qobject_cast<QWebSocket *>(sender());
    if (!client)
        return;
    
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    if (!doc.isObject()) {
        qWarning() << "Received invalid JSON";
        return;
    }
    
    handleCommand(client, doc.object());
}

void Server::onSocketDisconnected()
{
    QWebSocket *client = qobject_cast<QWebSocket *>(sender());
    if (client) {
        m_clients.removeAll(client);
        client->deleteLater();
        qDebug() << "Client disconnected";
    }
}

void Server::handleCommand(QWebSocket *client, const QJsonObject &command)
{
    QString type = command["type"].toString();
    
    QJsonObject response;
    response["id"] = command["id"];
    
    if (type == "media") {
        QString action = command["action"].toString();
        m_mediaController->handleAction(action, command);
        response["status"] = "success";
    }
    else if (type == "input") {
        QString action = command["action"].toString();
        m_inputController->handleAction(action, command);
        response["status"] = "success";
    }
    else if (type == "file") {
        m_fileTransfer->handleRequest(command, client);
        return; // File transfer handles its own response
    }
    else if (type == "system") {
        QString action = command["action"].toString();
        m_systemController->handleAction(action);
        response["status"] = "success";
    }
    else if (type == "screen") {
        m_screenShare->handleRequest(command, client);
        return; // Screen share handles its own response
    }
    else {
        response["status"] = "error";
        response["message"] = "Unknown command type";
    }
    
    client->sendTextMessage(QJsonDocument(response).toJson(QJsonDocument::Compact));
}
