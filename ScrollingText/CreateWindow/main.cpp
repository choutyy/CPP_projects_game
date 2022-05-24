#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

int main() {
	// Declare text
	sf::Font font;
	if (font.loadFromFile("msjh.ttc") == false) { return EXIT_FAILURE; }	
	sf::Text text("Hello", font);
	
	// Characteristics of text 
	sf::Color color(255, 0, 0); // red 
	text.setFillColor(color);
	text.setCharacterSize(100);	
	// Create window
	sf::RenderWindow window(sf::VideoMode (800, 600), "HELLO"); 

	// direction and speed
	enum class Direction { LEFT_TO_RIGHT, RIGHT_TO_LEFT };
	enum class Speed {SLOW, SPEED};
	Direction direction = Direction::LEFT_TO_RIGHT;
	if (direction == Direction::RIGHT_TO_LEFT) {
		text.setPosition((float)window.getSize().x, 0);
		text.setString(L"你好");
	}
	int counter = 0;

	while (window.isOpen()) {
		sf::Event evt; 
		if (window.pollEvent(evt)) {
			if (evt.type == evt.Closed) {
				window.close();
			}
			if (evt.type == sf::Event::KeyPressed) {
				switch (direction) {
					case Direction::LEFT_TO_RIGHT:
						direction = Direction::RIGHT_TO_LEFT;
						break;
					case Direction::RIGHT_TO_LEFT:
						direction = Direction::LEFT_TO_RIGHT;
						break;

				}
			}
		}
		// window actions
		window.clear();
		window.draw(text);
		window.display();
		//
		sf::Vector2f Position = text.getPosition();

		if (Position.x >= 800.00 || Position.x < 0.0) {
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
	
		switch (direction) {
		case Direction::LEFT_TO_RIGHT:
			text.move(0.1f, 0.f);
			text.setString("Hello");
			break;
		case Direction::RIGHT_TO_LEFT:
			text.move(-0.1f, 0.f);
			text.setString(L"你好");
			break;
		}
	}
	return EXIT_SUCCESS;
}


