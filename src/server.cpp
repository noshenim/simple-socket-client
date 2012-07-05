using boost::asio::ip::tcp;

class Server
{
public:
	Server(boost::asio::io_service &io_service);
	void bind(const std::string &service);
	void close();
private:
	boost::asio::io_service &io_service;
	std::list<tcp::socket*> socket_list;
};

Server::Server
