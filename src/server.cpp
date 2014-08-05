#include <iostream>
#include <rapidxml/rapidxml.hpp>
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
	message_ = "hello client\n";

	data_size.resize(1);

	boost::asio::async_read(socket_, boost::asio::buffer(data_size),
		boost::bind(&tcp_connection::handle_write, shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));

	data.resize(data_size[0]);
	std::cout << "received header, allocating " << data.size() << " of space" << std::endl;

	boost::asio::async_read(socket_, boost::asio::buffer(data),
		boost::bind(&tcp_connection::handle_xml, shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));

	boost::asio::async_write(socket_, boost::asio::buffer(message_),
		boost::bind(&tcp_connection::handle_write, shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
}

void tcp_connection::handle_xml(const boost::system::error_code&, size_t)
{
	std::string data_string = std::string(data.begin(), data.end());

	std::cout << "we received data" << std::endl;
	std::cout << data_string << std::endl;

	rapidxml::xml_document<> xml_data;
	xml_data.parse<0>(data.data()); //yeah i know bad name
	std::cout << "we parsed the data" << std::endl;
	std::cout << "first node is: " << xml_data.first_node()->name() << std::endl;
	std::cout << "its value is: " << xml_data.first_node()->value() << std::endl;
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
