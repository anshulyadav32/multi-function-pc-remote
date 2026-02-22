// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 Multi-Function PC Remote Contributors

#ifndef PC_REMOTE_CONNECTION_H
#define PC_REMOTE_CONNECTION_H

#include <glib-object.h>

G_BEGIN_DECLS

#define PC_REMOTE_TYPE_CONNECTION (pc_remote_connection_get_type())
G_DECLARE_FINAL_TYPE(PcRemoteConnection, pc_remote_connection, PC_REMOTE, CONNECTION, GObject)

PcRemoteConnection *pc_remote_connection_new(void);
gboolean pc_remote_connection_connect(PcRemoteConnection *self, const char *address);
void pc_remote_connection_disconnect(PcRemoteConnection *self);
void pc_remote_connection_send_command(PcRemoteConnection *self, 
                                      const char *type,
                                      const char *action,
                                      GHashTable *data);

G_END_DECLS

#endif /* PC_REMOTE_CONNECTION_H */
