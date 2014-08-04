#include <SFML/Graphics.hpp>
#include <boost/asio.hpp>
#include "server.hpp"

int main()
{
	boost::asio::io_service io_service;
	tcp_server server(io_service);

	sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

	while(window.isOpen())
	{
		sf::Event event;
		while(window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed)
			{
				std::cout << "ending due to window close" << std::endl;
				window.close();
			}
		}

		window.clear();
		window.draw(shape);
		window.display();

		io_service.poll();
	}

	return 0;
}
