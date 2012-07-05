#include <string>
#include <list>

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

class ClientUI;

class SendListener
{
friend class ClientUI;
protected:
	virtual void on_send(const std::string &message);
private:
	void send(const std::string &message);
};

class ConnectListener
{
friend class ClientUI;
protected:
	virtual void on_connect(
			const std::string &host, 
			const std::string &service);
private:
	void connect(const std::string &host, 
		     const std::string &service);
};

class ClientUI
{
public:
	ClientUI();
	~ClientUI();
	void display();
	void add_send_listener(SendListener &listener);
	void remove_send_listener(SendListener &listener);
	void add_connect_listener(ConnectListener &listener);
	void remove_connect_listener(ConnectListener &listener);
	void on_receive(const std::string &message);
private:
	std::list<SendListener*> send_listener_list;
	std::list<SendListener*> send_listener_remove_list;
	std::list<ConnectListener*> connect_listener_list;
	std::list<ConnectListener*> connect_listener_remove_list;
	GtkWidget *main_window;
	GtkWidget *main_box;
	GtkWidget *top_line;
	GtkWidget *host_name;
	GtkWidget *service_text;
	GtkWidget *connect_button;
	GtkWidget *client_text;
	GtkWidget *bot_line;
	GtkWidget *send_text;
	GtkWidget *send_button;
	static gboolean connect_text_key_press_event(
			GtkWidget *widget, 
			GdkEventKey *event, 
			ClientUI *client_ui);
	static gboolean connect_button_clicked(
			GtkWidget *widget,
			ClientUI *client_ui);
	static gboolean send_text_key_press_event(
			GtkWidget *widget, 
			GdkEventKey *event, 
			ClientUI *client_ui);
	static gboolean send_button_clicked(
			GtkWidget *widget, 
			ClientUI *client_ui);
	static gboolean main_window_destroy(
			GtkWidget *widget, 
			ClientUI *client_ui);
	void submit_connect();
	void send_message();
};


