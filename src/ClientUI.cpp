#include "ClientUI.hpp"

void SendListener::on_send(const std::string &message)
{
}

void SendListener::send(const std::string &message)
{
	on_send(message);
}

void ConnectListener::on_connect(const std::string &host, 
						const std::string &service)
{
}

void ConnectListener::connect(const std::string &host, 
						const std::string &service)
{
	on_connect(host, service);
}

void ClientUI::display()
{
	main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	gtk_window_set_default_size(GTK_WINDOW(main_window), 800, 600);
	gtk_window_set_title(GTK_WINDOW(main_window), "Socket UI");
	gtk_window_set_position(GTK_WINDOW(main_window), GTK_WIN_POS_CENTER);

	main_box = gtk_vbox_new(FALSE, 0);
	top_line = gtk_hbox_new(FALSE, 0);
	host_name = gtk_entry_new();
	service_text = gtk_entry_new();
	connect_button = gtk_button_new_with_label("connect");
	chat_text = gtk_text_view_new();
	bot_line = gtk_hbox_new(FALSE, 0);
	send_text = gtk_entry_new();
	send_button = gtk_button_new_with_label("send");

	gtk_text_view_set_editable(GTK_TEXT_VIEW(chat_text), FALSE);
	gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(chat_text), FALSE);

	gtk_container_add(GTK_CONTAINER(top_line), host_name);
	gtk_container_add(GTK_CONTAINER(top_line), service_text);
	gtk_container_add(GTK_CONTAINER(top_line), connect_button);
	gtk_box_pack_start(GTK_BOX(main_box), top_line, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(main_box), chat_text, TRUE, TRUE, 0);
	gtk_container_add(GTK_CONTAINER(bot_line), send_text);
	gtk_container_add(GTK_CONTAINER(bot_line), send_button);
	gtk_box_pack_start(GTK_BOX(main_box), bot_line, FALSE, FALSE, 0);
	gtk_container_add(GTK_CONTAINER(main_window), main_box);

	g_signal_connect(GTK_OBJECT(connect_button), "clicked", G_CALLBACK(
					&ClientUI::connect_button_clicked),
									this);
	g_signal_connect(GTK_OBJECT(send_text), "key-press-event", G_CALLBACK(
					&ClientUI::send_text_key_press_event),
									this);
	g_signal_connect(GTK_OBJECT(send_button), "clicked", G_CALLBACK(
					&ClientUI::send_button_clicked), 
									this);
	g_signal_connect(GTK_OBJECT(main_window), "destroy", G_CALLBACK(
					&ClientUI::main_window_destroy), 
									this);

	gtk_widget_show_all(main_window);
}

void ClientUI::add_connect_listener(ConnectListener &listener)
{
	connect_listener_list.push_back(&listener);
}

void ClientUI::remove_connect_listener(ConnectListener &listener)
{
	connect_listener_list.remove(&listener);
}

void ClientUI::add_send_listener(SendListener &listener)
{
	send_listener_list.push_back(&listener);
}

void ClientUI::remove_send_listener(SendListener &listener)
{
	send_listener_list.remove(&listener);
}

gboolean ClientUI::connect_button_clicked(	GtkWidget *widget, 
						ClientUI *client_ui)
{
	client_ui->connect_clicked();
}

gboolean ClientUI::send_text_key_press_event(	GtkWidget *widget, 
						GdkEventKey *event, 
						ClientUI *chat_ui)
{
	if (event->keyval == GDK_KEY_Return) {
		chat_ui->send_message();
	}
	return FALSE;
}

gboolean ClientUI::send_button_clicked(GtkWidget *widget, ClientUI *chat_ui)
{
	chat_ui->send_message();
}

gboolean ClientUI::main_window_destroy(GtkWidget *widget, ClientUI *chat_ui)
{
	gtk_main_quit();
}

void ClientUI::connect_clicked()
{
	const gchar *host = gtk_entry_get_text(GTK_ENTRY(host_name));
	guint16 host_len = gtk_entry_get_text_length(GTK_ENTRY(host_name));
	const gchar *service = gtk_entry_get_text(GTK_ENTRY(service_text));
	guint16 serv_len = gtk_entry_get_text_length(GTK_ENTRY(service_text));
	std::list<ConnectListener*>::iterator it;
	for (		it = connect_listener_list.begin(); 
			it != connect_listener_list.end(); 
			++it) {
		(*it)->connect(	std::string(host, host_len), 
				std::string(service, serv_len));
	}
}

void ClientUI::send_message()
{
	const gchar *text = gtk_entry_get_text(GTK_ENTRY(send_text));
	guint16 len = gtk_entry_get_text_length(GTK_ENTRY(send_text));
	/* the following two lines take the text from the input,
	 * and write it the output text area (chat with yourself). */
	#if 0
	GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(
								chat_text));
	gtk_text_buffer_insert_at_cursor(buffer, text, len);
	#endif
	std::list<SendListener*>::iterator it;
	for (it = send_listener_list.begin(); it != send_listener_list.end();
								++it) {
		(*it)->send(std::string(text, len));
	}
	gtk_entry_set_text(GTK_ENTRY(send_text), "");
}