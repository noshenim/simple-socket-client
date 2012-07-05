#include <iostream>

#include <boost/asio.hpp>

#include "client.hpp"

class STDOutReceiveListener
 : public ReceiveListener
{
public:
	void on_receive(const std::string &message);
};

void STDOutReceiveListener::on_receive(const std::string &message)
{
	std::cout << message;
	std::cout.flush();
}

int main(int argc, char *argv[])
{
	if (argc != 3) {
		std::cout << "Usage: " << argv[0] << " <host> <service>"
			  << std::endl;
		return 1;
	}
	boost::asio::io_service io_service;
	Client client(io_service);
	STDOutReceiveListener listener;
	client.add_receive_listener(listener);
	client.connect(argv[1], argv[2]);
	io_service.run();
	return 0;
}
