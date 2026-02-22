// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 Multi-Function PC Remote Contributors

#pragma once

#include <QObject>

class SystemController : public QObject
{
    Q_OBJECT

public:
    explicit SystemController(QObject *parent = nullptr);
    
    void handleAction(const QString &action);

private:
    void shutdown();
    void restart();
    void sleep();
    void lock();
};
