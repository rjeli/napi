#include "server.hpp"

tcp_connection::pointer tcp_connection::create(boost::asio::io_service& io_service)
{
	return tcp_connection::pointer(new tcp_connection(io_service));
}

tcp::socket& tcp_connection::socket()
{
	return socket_;
}

void tcp_connection::start()
{
	boost::asio::async_write(socket_, boost::asio::buffer("hello client\n"),
		boost::bind(&tcp_connection::handle_write, shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
}

void tcp_server::start_accept()
{
	tcp_connection::pointer new_connection =
		tcp_connection::create(acceptor_.get_io_service());

	acceptor_.async_accept(new_connection->socket(),
		boost::bind(&tcp_server::handle_accept, this, new_connection,
			boost::asio::placeholders::error));
}

void tcp_server::handle_accept(tcp_connection::pointer new_connection,
	const boost::system::error_code& error)
{
	if(!error)
	{
		new_connection->start();
	}

	start_accept();
}

void tcp_connection::handle_write(const boost::system::error_code&, size_t)
{

}
