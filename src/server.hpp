#ifndef __SERVER_H
#define __SERVER_H

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class tcp_connection
	: public boost::enable_shared_from_this<tcp_connection>
{
public:
	typedef boost::shared_ptr<tcp_connection> pointer;
	static pointer create(boost::asio::io_service&);
	tcp::socket& socket();
	void start();

private:
	tcp_connection(boost::asio::io_service& io_service)
		: socket_(io_service){};
	void handle_write(const boost::system::error_code&, size_t);

	tcp::socket socket_;
	std::string message_;
};

class tcp_server
{
public:
	tcp_server(boost::asio::io_service& io_service)
		: acceptor_(io_service, tcp::endpoint(tcp::v4(), 13))
	{
		start_accept();
	}

private:
	void start_accept();
	void handle_accept(tcp_connection::pointer,
		const boost::system::error_code&);

	tcp::acceptor acceptor_;
};

#endif
