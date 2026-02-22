// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 Multi-Function PC Remote Contributors

#include "connection.h"
#include <libsoup/soup.h>
#include <json-glib/json-glib.h>

struct _PcRemoteConnection
{
    GObject parent_instance;
    
    SoupSession *session;
    SoupWebsocketConnection *websocket;
    gboolean connected;
};

G_DEFINE_TYPE(PcRemoteConnection, pc_remote_connection, G_TYPE_OBJECT)

static void
on_message_received(SoupWebsocketConnection *ws,
                   SoupWebsocketDataType type,
                   GBytes *message,
                   PcRemoteConnection *self)
{
    if (type != SOUP_WEBSOCKET_DATA_TEXT)
        return;
    
    gsize size;
    const char *data = g_bytes_get_data(message, &size);
    
    g_autoptr(JsonParser) parser = json_parser_new();
    if (json_parser_load_from_data(parser, data, size, NULL)) {
        JsonNode *root = json_parser_get_root(parser);
        JsonObject *obj = json_node_get_object(root);
        
        const char *msg_type = json_object_get_string_member(obj, "type");
        g_print("Received message type: %s\n", msg_type);
        
        // Handle different message types here
    }
}

static void
on_websocket_closed(SoupWebsocketConnection *ws,
                   PcRemoteConnection *self)
{
    g_print("WebSocket connection closed\n");
    self->connected = FALSE;
    g_clear_object(&self->websocket);
}

static void
on_websocket_connected(GObject *source,
                      GAsyncResult *res,
                      gpointer user_data)
{
    PcRemoteConnection *self = PC_REMOTE_CONNECTION(user_data);
    g_autoptr(GError) error = NULL;
    
    self->websocket = soup_session_websocket_connect_finish(self->session, res, &error);
    
    if (error) {
        g_warning("Failed to connect: %s", error->message);
        self->connected = FALSE;
        return;
    }
    
    self->connected = TRUE;
    g_print("Connected to server\n");
    
    g_signal_connect(self->websocket, "message",
                    G_CALLBACK(on_message_received), self);
    g_signal_connect(self->websocket, "closed",
                    G_CALLBACK(on_websocket_closed), self);
}

static void
pc_remote_connection_dispose(GObject *object)
{
    PcRemoteConnection *self = PC_REMOTE_CONNECTION(object);
    
    if (self->websocket) {
        soup_websocket_connection_close(self->websocket, SOUP_WEBSOCKET_CLOSE_NORMAL, NULL);
        g_clear_object(&self->websocket);
    }
    
    g_clear_object(&self->session);
    
    G_OBJECT_CLASS(pc_remote_connection_parent_class)->dispose(object);
}

static void
pc_remote_connection_class_init(PcRemoteConnectionClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    object_class->dispose = pc_remote_connection_dispose;
}

static void
pc_remote_connection_init(PcRemoteConnection *self)
{
    self->session = soup_session_new();
    self->connected = FALSE;
}

PcRemoteConnection *
pc_remote_connection_new(void)
{
    return g_object_new(PC_REMOTE_TYPE_CONNECTION, NULL);
}

gboolean
pc_remote_connection_connect(PcRemoteConnection *self, const char *address)
{
    g_return_val_if_fail(PC_REMOTE_IS_CONNECTION(self), FALSE);
    g_return_val_if_fail(address != NULL, FALSE);
    
    g_autofree char *uri_string = g_strdup_printf("ws://%s", address);
    g_autoptr(SoupMessage) msg = soup_message_new("GET", uri_string);
    
    soup_session_websocket_connect_async(self->session,
                                        msg,
                                        NULL, NULL, 0,
                                        NULL,
                                        on_websocket_connected,
                                        self);
    
    return TRUE;
}

void
pc_remote_connection_disconnect(PcRemoteConnection *self)
{
    g_return_if_fail(PC_REMOTE_IS_CONNECTION(self));
    
    if (self->websocket) {
        soup_websocket_connection_close(self->websocket, SOUP_WEBSOCKET_CLOSE_NORMAL, NULL);
        g_clear_object(&self->websocket);
    }
    
    self->connected = FALSE;
}

void
pc_remote_connection_send_command(PcRemoteConnection *self,
                                 const char *type,
                                 const char *action,
                                 GHashTable *data)
{
    g_return_if_fail(PC_REMOTE_IS_CONNECTION(self));
    g_return_if_fail(self->connected);
    g_return_if_fail(type != NULL);
    g_return_if_fail(action != NULL);
    
    JsonBuilder *builder = json_builder_new();
    json_builder_begin_object(builder);
    
    json_builder_set_member_name(builder, "type");
    json_builder_add_string_value(builder, type);
    
    json_builder_set_member_name(builder, "action");
    json_builder_add_string_value(builder, action);
    
    json_builder_set_member_name(builder, "id");
    json_builder_add_int_value(builder, g_get_real_time() / 1000);
    
    if (data) {
        GHashTableIter iter;
        gpointer key, value;
        
        g_hash_table_iter_init(&iter, data);
        while (g_hash_table_iter_next(&iter, &key, &value)) {
            json_builder_set_member_name(builder, key);
            json_builder_add_string_value(builder, value);
        }
    }
    
    json_builder_end_object(builder);
    
    JsonNode *root = json_builder_get_root(builder);
    JsonGenerator *gen = json_generator_new();
    json_generator_set_root(gen, root);
    
    g_autofree char *json_string = json_generator_to_data(gen, NULL);
    
    soup_websocket_connection_send_text(self->websocket, json_string);
    
    json_node_free(root);
    g_object_unref(gen);
    g_object_unref(builder);
}
