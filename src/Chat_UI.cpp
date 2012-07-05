#include <gtk/gtk.h>
#include <cstdlib>
#include <gdk/gdkkeysyms.h>

class ChatUI;

class SendListener
{
friend class ChatUI;
protected:
	virtual void on_send(const std::string &message);
private:
	void send(const std::string &message);
};

class ChatUI
{
public:
	void display();
	void add_send_listener(SendListener &listener);
	void remove_send_listener(SendListener &listener);
private:
	std::list<SendListener*> send_listener_list;
	GtkWidget *main_window;
	GtkWidget *main_box;
	GtkWidget *top_line;
	GtkWidget *host_name_text;
	GtkWidget *port_text;
	GtkWidget *connect_button;
	GtkWidget *chat_text;
	GtkWidget *bot_line;
	GtkWidget *send_text;
	GtkWidget *send_button;
	static gboolean send_text_key_press_event(GtkWidget *widget, ChatUI 
								*chat_ui);
	static gboolean send_button_clicked(GtkWidget *widget, ChatUI 
								*chat_ui);
	static gboolean main_window_destroy(GtkWidget *widget, ChatUI 
								*chat_ui);
	void send_message();
};

void SendListener::on_send(const std::string &message)
{
}

void SendListener::send(const std::string &message)
{
	on_send(message);
}

void ChatUI::display()
{
	main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	gtk_window_set_default_size(GTK_WINDOW(main_window), 800, 600);
	gtk_window_set_title(GTK_WINDOW(main_window), "Socket UI");
	gtk_window_set_position(GTK_WINDOW(main_window), GTK_WIN_POS_CENTER);

	main_box = gtk_vbox_new(FALSE, 0);
	top_line = gtk_hbox_new(FALSE, 0);
	host_name_text = gtk_entry_new();
	port_text = gtk_entry_new();
	connect_button = gtk_button_new_with_label("connect");
	chat_text = gtk_text_view_new();
	bot_line = gtk_hbox_new(FALSE, 0);
	send_text = gtk_entry_new();
	send_button = gtk_button_new_with_label("send");

	gtk_text_view_set_editable(GTK_TEXT_VIEW(chat_text), FALSE);
	gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(chat_text), FALSE);

	gtk_container_add(GTK_CONTAINER(top_line), host_name_text);
	gtk_container_add(GTK_CONTAINER(top_line), port_text);
	gtk_container_add(GTK_CONTAINER(top_line), connect_button);
	gtk_box_pack_start(GTK_BOX(main_box), top_line, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(main_box), chat_text, TRUE, TRUE, 0);
	gtk_container_add(GTK_CONTAINER(bot_line), send_text);
	gtk_container_add(GTK_CONTAINER(bot_line), send_button);
	gtk_box_pack_start(GTK_BOX(main_box), bot_line, FALSE, FALSE, 0);
	gtk_container_add(GTK_CONTAINER(mainWindow), main_box);

	g_signal_connect(GTK_OBJECT(send_text), "key-press-event", G_CALLBACK(
					&ChatUI::send_text_key_press_event),
									this);
	g_signal_connect(GTK_OBJECT(send_button), "clicked", G_CALLBACK(
					&ChatUI::send_button_clocked), this);
	g_signal_connect(GTK_OBJECT(main_window), "destroy", G_CALLBACK(
					&ChatUI::main_window_destroy), this);
	
	gtk_widget_show_all(main_window);
}

void ChatUI::add_send_listener(SendListener &listener)
{
	send_listener_list.push_back(&listener);
}

void ChatUI::remove_send_listener(SendListener &listener)
{
	send_listener_list.remove(&listener);
}

gboolean ChatUI::send_text_key_press_event(GtkWidget *widget, ChatUI *chat_ui)
{
	if (event->keyval == GDK_KEY_Return) {
		chat_ui->send_message();
	}
	return FALSE;
}

gboolean ChatUI::send_button_clicked(GtkWidget *widget, ChatUI *chat_ui)
{
	chat_ui->send_message();
}

void ChatUI::send_message()
{
	GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(
								chat_text));
	guint16 len = gtk_entry_get_text_length(GTK_ENTRY(send_text));
	/* the following two lines take the text from the input,
	 * and write it the output text area (chat with yourself). */
	#if 0
	const gchar *text = gtk_entry_get_text(GTK_ENTRY(send_text));
	gtk_text_buffer_insert_at_cursor(buffer, text, len);
	#endif
	std::list<SendListner*>::iterator it;
	for (it = send_listener_list.begin(); it != send_listener_list.end();
								++it) {
		(*it)->send(text, len);
	}
	gtk_entry_set_text(GTK_ENTRY(send_text), "");
}

#if 0
int main(int argc, char *argv[])
{
	gtk_init(&argc, &argv);
	ChatUI ui;
	ui.display();
	gtk_main();
}
#endif
