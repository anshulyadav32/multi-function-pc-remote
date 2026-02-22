// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 Multi-Function PC Remote Contributors

#ifndef PC_REMOTE_WINDOW_H
#define PC_REMOTE_WINDOW_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define PC_REMOTE_TYPE_WINDOW (pc_remote_window_get_type())
G_DECLARE_FINAL_TYPE(PcRemoteWindow, pc_remote_window, PC_REMOTE, WINDOW, GtkApplicationWindow)

PcRemoteWindow *pc_remote_window_new(GtkApplication *app);

G_END_DECLS

#endif /* PC_REMOTE_WINDOW_H */
