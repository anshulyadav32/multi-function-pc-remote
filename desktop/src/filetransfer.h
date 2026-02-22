// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 Multi-Function PC Remote Contributors

#pragma once

#include <QObject>
#include <QJsonObject>

class QWebSocket;

class FileTransfer : public QObject
{
    Q_OBJECT

public:
    explicit FileTransfer(QObject *parent = nullptr);
    
    void handleRequest(const QJsonObject &request, QWebSocket *client);

private:
    void sendFile(const QString &filePath, QWebSocket *client);
    void receiveFile(const QJsonObject &request, QWebSocket *client);
};
