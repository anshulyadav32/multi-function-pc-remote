// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 Multi-Function PC Remote Contributors

#include "filetransfer.h"
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QWebSocket>
#include <QDebug>

FileTransfer::FileTransfer(QObject *parent)
    : QObject(parent)
{
}

void FileTransfer::handleRequest(const QJsonObject &request, QWebSocket *client)
{
    QString action = request["action"].toString();
    
    if (action == "send") {
        QString filePath = request["path"].toString();
        sendFile(filePath, client);
    } else if (action == "receive") {
        receiveFile(request, client);
    }
}

void FileTransfer::sendFile(const QString &filePath, QWebSocket *client)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        QJsonObject response;
        response["type"] = "file";
        response["status"] = "error";
        response["message"] = "Failed to open file";
        client->sendTextMessage(QJsonDocument(response).toJson());
        return;
    }
    
    QFileInfo fileInfo(file);
    QByteArray fileData = file.readAll();
    
    QJsonObject response;
    response["type"] = "file";
    response["action"] = "data";
    response["filename"] = fileInfo.fileName();
    response["size"] = fileData.size();
    response["data"] = QString::fromLatin1(fileData.toBase64());
    
    client->sendTextMessage(QJsonDocument(response).toJson());
    qDebug() << "File sent:" << filePath;
}

void FileTransfer::receiveFile(const QJsonObject &request, QWebSocket *client)
{
    QString filename = request["filename"].toString();
    QString dataBase64 = request["data"].toString();
    
    QByteArray fileData = QByteArray::fromBase64(dataBase64.toLatin1());
    
    QString savePath = QDir::home().filePath(filename);
    QFile file(savePath);
    
    if (file.open(QIODevice::WriteOnly)) {
        file.write(fileData);
        file.close();
        
        QJsonObject response;
        response["type"] = "file";
        response["status"] = "success";
        response["message"] = "File received";
        client->sendTextMessage(QJsonDocument(response).toJson());
        
        qDebug() << "File received:" << savePath;
    } else {
        QJsonObject response;
        response["type"] = "file";
        response["status"] = "error";
        response["message"] = "Failed to save file";
        client->sendTextMessage(QJsonDocument(response).toJson());
    }
}
