#include <iostream>

#include "ClientUI.hpp"

class Debugger: public SendListener, public ConnectListener
{
protected:
	void on_send(const std::string &message);
	void on_connect(const std::string &host, const std::string &service);
};

void Debugger::on_send(const std::string &message)
{
	std::cout << "Message: " << message << std::endl;
}

void Debugger::on_connect(const std::string &host, const std::string &service)
{
	std::cout << "Host, Service: " << host << ", " << service;
	std::cout << std::endl;
}

int main(int argc, char *argv[])
{
	gtk_init(&argc, &argv);

	ClientUI ui;
	Debugger d;
	ui.add_send_listener(d);
	ui.add_connect_listener(d);
	ui.display();

	gtk_main();

	return 0;
}
