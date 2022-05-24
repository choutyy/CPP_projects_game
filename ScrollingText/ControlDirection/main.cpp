#include<iostream>
#include <fstream>
#include<SFML\Window.hpp>
#include<SFML\Graphics.hpp>

int main() {
	sf::Font font;
	if (font.loadFromFile("msjh.ttc") == false) { return EXIT_FAILURE; }

	std::string line;
	std::ifstream file;

	sf::Text text("", font);
	file.open("input.txt"); 
	if (file.is_open())
	{
		while (std::getline(file, line))
		{
			text.setString(line);
		}
	}

	sf::Color color(255, 0, 0); // red 
	text.setFillColor(color);
	text.setCharacterSize(100);

	sf::CircleShape circle((float)2);
	circle.setFillColor(sf::Color::Black); // or (0, 0, 0)

	enum class Direction { LEFT_TO_RIGHT, RIGHT_TO_LEFT };
	enum class Speed { SLOW, SLOW_NOR, NORMAL, NOR_FAST, FAST, ADJ};

	// default values
	Direction direction = Direction::LEFT_TO_RIGHT;
	Speed speed = Speed::NORMAL;
	float diff = 0.1f;
	int counter = 0;
	sf::RenderWindow window(sf::VideoMode(2000, 600), "HW3");

	//window function
	while (window.isOpen()) {
		sf::Event evt;
		
		if (window.pollEvent(evt)) {
			if (evt.type == evt.Closed) { 
				window.close(); 
			}
			if (evt.type == evt.KeyPressed) {
				int key = evt.key.code;
				std::cout << evt.key.code << std::endl;
				switch (key) {
				case 5: // F
					speed = Speed::FAST;
					break;
				case 31: // #5
					speed = Speed::FAST;
					break;
				case 30: // #4
					speed = Speed::NOR_FAST;
					break;
				case 29: // #3
					speed = Speed::NORMAL;
					break;
				case 13: // N
					speed = Speed::NORMAL;
					break;
				case 28: // #2
					speed = Speed::SLOW_NOR;
					break;
				case 27: // #1
					speed = Speed::SLOW;
					break;
				case 18: // S
					speed = Speed::SLOW;
					break;
				}
				if (key == 55) { speed = Speed::ADJ; diff = diff * 2; }
				if (key == 56) { speed = Speed::ADJ; diff = diff * 0.5; }
			}
		}
		window.clear();
		window.draw(text);
		//window.draw(circle); // Draw the circle full screen
		window.display();

		sf::Vector2f Position = text.getPosition();
		if (Position.x >= window.getSize().x || Position.x < 0.0) {
			counter++;
			if (counter % 2 == 1) {
				text.setPosition((float)window.getSize().x, 0);
				direction = Direction::RIGHT_TO_LEFT;
			}
			else {
				text.setPosition(0, 0);
				direction = Direction::LEFT_TO_RIGHT;
			}
		}

		// switch for direction and speed	
		switch (speed) {
		case Speed::FAST:
			diff = 0.5f;
			break;
		case Speed::NOR_FAST:
			diff = 0.35f;
			break;
		case Speed::NORMAL:
			diff = 0.1f;
			break;
		case Speed::SLOW_NOR:
			diff = 0.08f;
			break;
		case Speed::SLOW:
			diff = 0.05f;
			break;
		}

		switch (direction) {
		case Direction::LEFT_TO_RIGHT:
			text.move(diff, 0.f);
			break;
		case Direction::RIGHT_TO_LEFT:
			text.move(-diff, 0.f);
			break;
		}	
	}		
	return EXIT_SUCCESS;
}