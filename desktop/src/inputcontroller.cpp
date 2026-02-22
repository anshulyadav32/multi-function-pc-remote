// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 Multi-Function PC Remote Contributors

#include "inputcontroller.h"
#include <QDebug>
#include <QCursor>
#include <QGuiApplication>
#include <QScreen>

InputController::InputController(QObject *parent)
    : QObject(parent)
{
}

void InputController::handleAction(const QString &action, const QJsonObject &data)
{
    if (action == "mouse_move") {
        int deltaX = data["deltaX"].toInt();
        int deltaY = data["deltaY"].toInt();
        moveMouse(deltaX, deltaY);
    } else if (action == "mouse_click") {
        QString button = data["button"].toString();
        mouseClick(button);
    } else if (action == "key") {
        QString key = data["key"].toString();
        sendKey(key);
    } else if (action == "text") {
        QString text = data["text"].toString();
        sendText(text);
    }
}

void InputController::moveMouse(int deltaX, int deltaY)
{
    QPoint currentPos = QCursor::pos();
    QCursor::setPos(currentPos.x() + deltaX, currentPos.y() + deltaY);
    qDebug() << "Mouse moved by" << deltaX << deltaY;
}

void InputController::mouseClick(const QString &button)
{
    // Mouse click implementation would use platform-specific APIs
    qDebug() << "Mouse click:" << button;
}

void InputController::sendKey(const QString &key)
{
    // Keyboard input implementation would use platform-specific APIs
    qDebug() << "Key pressed:" << key;
}

void InputController::sendText(const QString &text)
{
    // Text input implementation
    qDebug() << "Text sent:" << text;
}
