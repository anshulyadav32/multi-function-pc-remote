// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 Multi-Function PC Remote Contributors

#pragma once

#include <QObject>
#include <QJsonObject>

class InputController : public QObject
{
    Q_OBJECT

public:
    explicit InputController(QObject *parent = nullptr);
    
    void handleAction(const QString &action, const QJsonObject &data);

private:
    void moveMouse(int deltaX, int deltaY);
    void mouseClick(const QString &button);
    void sendKey(const QString &key);
    void sendText(const QString &text);
};
