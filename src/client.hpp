#ifndef CLIENT_HPP_INCLUDE
#define CLIENT_HPP_INCLUDE

#include <string>
#include <list>
#include <cstdlib>

#include <boost/asio.hpp>
#include <boost/shared_array.hpp>

using boost::asio::ip::tcp;

class Client;

class ReceiveListener
{
friend class Client;
public:
	virtual void on_receive(const std::string &message);
private:
	void receive(const std::string &message);
};

class Client
{
public:
	Client(boost::asio::io_service &io_service);
	void connect(const std::string &host, const std::string &service);
	void disconnect();
	void send(const std::string &message);
	void add_receive_listener(ReceiveListener &listener);
	void remove_receive_listener(ReceiveListener &listener);
private:
	boost::asio::io_service &io_service;
	tcp::socket socket;
	std::list<ReceiveListener*> listener_list;
	void start_receive();
	void handle_receive(boost::shared_array<char> message, std::size_t len,
					const boost::system::error_code &ec);
};

#endif
