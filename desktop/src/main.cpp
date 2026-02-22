// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 Multi-Function PC Remote Contributors

#include <QApplication>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QMessageBox>
#include "server.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Multi-Function PC Remote");
    app.setApplicationVersion("1.0.0");
    app.setQuitOnLastWindowClosed(false);

    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(nullptr, "System Tray",
                            "System tray is not available on this system.");
        return 1;
    }

    Server server;
    if (!server.start(8765)) {
        QMessageBox::critical(nullptr, "Server Error",
                            "Failed to start server on port 8765");
        return 1;
    }

    QSystemTrayIcon trayIcon;
    trayIcon.setIcon(QIcon::fromTheme("network-server"));
    trayIcon.setToolTip("PC Remote Server - Running");

    QMenu trayMenu;
    QAction *statusAction = trayMenu.addAction("Server: Running on port 8765");
    statusAction->setEnabled(false);
    trayMenu.addSeparator();
    QAction *quitAction = trayMenu.addAction("Quit");
    
    QObject::connect(quitAction, &QAction::triggered, &app, &QApplication::quit);
    
    trayIcon.setContextMenu(&trayMenu);
    trayIcon.show();
    trayIcon.showMessage("PC Remote Server", 
                        "Server is running on port 8765",
                        QSystemTrayIcon::Information, 3000);

    return app.exec();
}
