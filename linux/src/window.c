// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 Multi-Function PC Remote Contributors

#include "window.h"
#include "connection.h"

struct _PcRemoteWindow
{
    GtkApplicationWindow parent_instance;
    
    GtkStack *main_stack;
    GtkStack *control_stack;
    GtkEntry *server_entry;
    GtkButton *connect_button;
    GtkButton *disconnect_button;
    
    PcRemoteConnection *connection;
};

G_DEFINE_TYPE(PcRemoteWindow, pc_remote_window, GTK_TYPE_APPLICATION_WINDOW)

static void
on_connect_clicked(GtkButton *button, PcRemoteWindow *self)
{
    const char *address = gtk_editable_get_text(GTK_EDITABLE(self->server_entry));
    
    if (pc_remote_connection_connect(self->connection, address)) {
        gtk_stack_set_visible_child_name(self->main_stack, "controls");
    }
}

static void
on_disconnect_clicked(GtkButton *button, PcRemoteWindow *self)
{
    pc_remote_connection_disconnect(self->connection);
    gtk_stack_set_visible_child_name(self->main_stack, "connect");
}

static void
pc_remote_window_dispose(GObject *object)
{
    PcRemoteWindow *self = PC_REMOTE_WINDOW(object);
    
    g_clear_object(&self->connection);
    
    G_OBJECT_CLASS(pc_remote_window_parent_class)->dispose(object);
}

static void
pc_remote_window_class_init(PcRemoteWindowClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    object_class->dispose = pc_remote_window_dispose;
}

static GtkWidget *
create_connect_page(PcRemoteWindow *self)
{
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 24);
    gtk_widget_set_margin_top(box, 48);
    gtk_widget_set_margin_bottom(box, 48);
    gtk_widget_set_margin_start(box, 48);
    gtk_widget_set_margin_end(box, 48);
    gtk_widget_set_valign(box, GTK_ALIGN_CENTER);
    
    GtkWidget *icon = gtk_image_new_from_icon_name("computer-symbolic");
    gtk_image_set_pixel_size(GTK_IMAGE(icon), 128);
    gtk_box_append(GTK_BOX(box), icon);
    
    GtkWidget *title = gtk_label_new("Connect to PC");
    gtk_widget_add_css_class(title, "title-1");
    gtk_box_append(GTK_BOX(box), title);
    
    GtkWidget *subtitle = gtk_label_new("Enter your PC's IP address and port");
    gtk_widget_add_css_class(subtitle, "dim-label");
    gtk_box_append(GTK_BOX(box), subtitle);
    
    self->server_entry = GTK_ENTRY(gtk_entry_new());
    gtk_entry_set_placeholder_text(self->server_entry, "192.168.1.100:8765");
    gtk_widget_set_size_request(GTK_WIDGET(self->server_entry), 300, -1);
    gtk_box_append(GTK_BOX(box), GTK_WIDGET(self->server_entry));
    
    self->connect_button = GTK_BUTTON(gtk_button_new_with_label("Connect"));
    gtk_widget_add_css_class(GTK_WIDGET(self->connect_button), "suggested-action");
    gtk_widget_set_size_request(GTK_WIDGET(self->connect_button), 300, -1);
    g_signal_connect(self->connect_button, "clicked", G_CALLBACK(on_connect_clicked), self);
    gtk_box_append(GTK_BOX(box), GTK_WIDGET(self->connect_button));
    
    return box;
}

static GtkWidget *
create_controls_page(PcRemoteWindow *self)
{
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    
    // Header bar for controls
    GtkWidget *header = gtk_header_bar_new();
    gtk_header_bar_set_title_widget(GTK_HEADER_BAR(header), gtk_label_new("PC Remote"));
    
    self->disconnect_button = GTK_BUTTON(gtk_button_new_with_label("Disconnect"));
    gtk_widget_add_css_class(GTK_WIDGET(self->disconnect_button), "destructive-action");
    gtk_header_bar_pack_end(GTK_HEADER_BAR(header), GTK_WIDGET(self->disconnect_button));
    g_signal_connect(self->disconnect_button, "clicked", G_CALLBACK(on_disconnect_clicked), self);
    
    gtk_box_append(GTK_BOX(box), header);
    
    // Stack switcher
    GtkWidget *switcher = gtk_stack_switcher_new();
    gtk_widget_set_halign(switcher, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(switcher, 12);
    gtk_widget_set_margin_bottom(switcher, 12);
    
    // Control stack
    self->control_stack = GTK_STACK(gtk_stack_new());
    gtk_stack_set_transition_type(self->control_stack, GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT_RIGHT);
    gtk_stack_switcher_set_stack(GTK_STACK_SWITCHER(switcher), self->control_stack);
    
    // Add pages (placeholders for now)
    gtk_stack_add_titled(self->control_stack, 
                        gtk_label_new("Media Controls"),
                        "media", "Media");
    gtk_stack_add_titled(self->control_stack,
                        gtk_label_new("Input Controls"),
                        "input", "Input");
    gtk_stack_add_titled(self->control_stack,
                        gtk_label_new("File Transfer"),
                        "files", "Files");
    gtk_stack_add_titled(self->control_stack,
                        gtk_label_new("System Commands"),
                        "system", "System");
    gtk_stack_add_titled(self->control_stack,
                        gtk_label_new("Screen Sharing"),
                        "screen", "Screen");
    
    gtk_box_append(GTK_BOX(box), switcher);
    gtk_box_append(GTK_BOX(box), GTK_WIDGET(self->control_stack));
    gtk_widget_set_vexpand(GTK_WIDGET(self->control_stack), TRUE);
    
    return box;
}

static void
pc_remote_window_init(PcRemoteWindow *self)
{
    gtk_window_set_title(GTK_WINDOW(self), "PC Remote");
    gtk_window_set_default_size(GTK_WINDOW(self), 600, 500);
    
    self->connection = pc_remote_connection_new();
    
    // Main stack
    self->main_stack = GTK_STACK(gtk_stack_new());
    gtk_stack_set_transition_type(self->main_stack, GTK_STACK_TRANSITION_TYPE_CROSSFADE);
    
    gtk_stack_add_named(self->main_stack, create_connect_page(self), "connect");
    gtk_stack_add_named(self->main_stack, create_controls_page(self), "controls");
    
    gtk_window_set_child(GTK_WINDOW(self), GTK_WIDGET(self->main_stack));
}

PcRemoteWindow *
pc_remote_window_new(GtkApplication *app)
{
    return g_object_new(PC_REMOTE_TYPE_WINDOW,
                       "application", app,
                       NULL);
}
