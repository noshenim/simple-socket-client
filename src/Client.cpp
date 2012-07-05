#define DEBUG(x) std::cout << x << std::endl;

#include <string>
#include <list>
#include <cstdlib>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_array.hpp>

#include "Client.hpp"

using boost::asio::ip::tcp;

void ReceiveListener::on_receive(const std::string &message)
{
}

void ReceiveListener::receive(const std::string &message)
{
	on_receive(message);
}

Client::Client(boost::asio::io_service &io_service)
 : io_service(io_service), socket(io_service)
{
	
}

void Client::connect(const std::string &host, const std::string &service)
{
	tcp::resolver resolver(io_service);
	tcp::resolver::query query(host, service);
	tcp::resolver::iterator end_it = resolver.resolve(query);
	tcp::resolver::iterator end;
	boost::system::error_code error = boost::asio::error::host_not_found;
	while (error && end_it != end) {
		socket.close();
		socket.connect(*end_it, error);
		++end_it;
	}
	if (error) throw boost::system::system_error(error);
	start_receive();
}

void Client::disconnect()
{
	socket.cancel();
}

void Client::send(const std::string &message)
{
	socket.send(boost::asio::buffer(message));
}

void Client::add_receive_listener(ReceiveListener &listener)
{
	listener_list.push_back(&listener);
}

void Client::remove_receive_listener(ReceiveListener &listener)
{
	listener_list.remove(&listener);
}

void Client::start_receive()
{
	std::size_t len = socket.available();
	boost::shared_array<char> message(new char[len]);
	socket.async_receive(boost::asio::buffer(message.get(), len), 
					boost::bind(
					&Client::handle_receive,
					this,
					message,
					len,
					boost::asio::placeholders::error));
}

void Client::handle_receive(boost::shared_array<char> message,
					std::size_t len,
					const boost::system::error_code &ec)
{
	std::list<ReceiveListener*>::iterator it;
	for (it = listener_list.begin(); it != listener_list.end(); ++it)
	{
		(*it)->receive(std::string(message.get(), len));
	}
	start_receive();
}
