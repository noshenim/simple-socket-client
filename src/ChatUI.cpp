#include <gtk/gtk.h>
#include <stdlib.h>
#include <gdk/gdkkeysyms.h>

class ChatUI;

gboolean onSendToChatUI(GtkWidget *widget, ChatUI *data);

gboolean onChatUISendKeyDown(GtkWidget *widget, GdkEventKey *event, ChatUI *data);

class ChatUI
{
friend gboolean onSendToChatUI(GtkWidget *widget, ChatUI *data);
friend gboolean onChatUISendKeyDown(GtkWidget *widget, GdkEventKey *event, ChatUI *data);
public:
    void display();
private:
    GtkWidget *mainBox;
    GtkWidget *topLine;
    GtkWidget *hostNameText;
    GtkWidget *portText;
    GtkWidget *connectButton;
    GtkWidget *chatText;
    GtkWidget *botLine;
    GtkWidget *sendText;
    GtkWidget *sendButton;
    void onSend();
};

gboolean onSendToChatUI(GtkWidget *widget, ChatUI *data)
{
    data->onSend();
}

gboolean onChatUISendKeyDown(GtkWidget *widget, GdkEventKey *event, ChatUI *data)
{
    if(event->keyval == GDK_KEY_Return)
    {
        onSendToChatUI(widget, data);
    }
    return FALSE;
}

void ChatUI::display()
{
    GtkWidget *mainWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_window_set_default_size(GTK_WINDOW(mainWindow), 400, 300);
    gtk_window_set_title(GTK_WINDOW(mainWindow), "GTK Simple Example");
    gtk_window_set_position(GTK_WINDOW(mainWindow), GTK_WIN_POS_CENTER);

    mainBox = gtk_vbox_new(FALSE, 0);
    topLine = gtk_hbox_new(FALSE, 0);
    hostNameText = gtk_entry_new();
    portText = gtk_entry_new();
    connectButton = gtk_button_new_with_label("connect");
    chatText = gtk_text_view_new();
    botLine = gtk_hbox_new(FALSE, 0);
    sendText = gtk_entry_new();
    sendButton = gtk_button_new_with_label("send");
 
    gtk_text_view_set_editable(GTK_TEXT_VIEW(chatText), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(chatText), FALSE);

    gtk_container_add(GTK_CONTAINER(topLine), hostNameText);
    gtk_container_add(GTK_CONTAINER(topLine), portText);
    gtk_container_add(GTK_CONTAINER(topLine), connectButton);
    gtk_box_pack_start(GTK_BOX(mainBox), topLine, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(mainBox), chatText, TRUE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(botLine), sendText);
    gtk_container_add(GTK_CONTAINER(botLine), sendButton);
    gtk_box_pack_start(GTK_BOX(mainBox), botLine, FALSE, FALSE, 0);
    gtk_container_add(GTK_CONTAINER(mainWindow), mainBox);
    
    g_signal_connect(GTK_OBJECT(sendText), "key-press-event", G_CALLBACK(onChatUISendKeyDown), this);
    g_signal_connect(GTK_OBJECT(sendButton), "clicked", G_CALLBACK(onSendToChatUI), this);
    g_signal_connect(GTK_OBJECT(mainWindow), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(mainWindow);
}

void ChatUI::onSend()
{
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(chatText));
    const gchar *text = gtk_entry_get_text(GTK_ENTRY(sendText));
    guint16 textLen = gtk_entry_get_text_length(GTK_ENTRY(sendText));
    gtk_text_buffer_insert_at_cursor(buffer, text, textLen);
    gtk_entry_set_text(GTK_ENTRY(sendText), "");
}

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    ChatUI ui;
    ui.display();

    gtk_main();
}
