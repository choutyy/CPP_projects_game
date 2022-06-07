#include<SFML/Graphics.hpp>
#include<iostream>

sf::Vector2f getPixelPosition(
	const sf::Vector2i& pos, 
	const sf::Vector2u& blockSize) {
	return { float(pos.x * blockSize.x), float(pos.y * blockSize.y) };
}
int main() {
	// field 
	const int fieldWidth = 11;
	const int fieldHeight = 18;
	bool field[fieldWidth][fieldHeight] = {};
	for (int j = 6; j < fieldHeight; j++) {
		for (int i = 1; i <= 3; i++) {
			field[rand() % fieldWidth][j] = true;
		}
	}
	// exture
	sf::Texture blockTexture;
	blockTexture.loadFromFile("block.png");
	sf::Vector2u blockSize = blockTexture.getSize();
	const unsigned int windowWidth = blockSize.x * fieldWidth;
	const unsigned int windowHeight = blockSize.y * fieldHeight;
	// window
	sf::String title(L"簡易消波塊");
	sf::VideoMode mode(windowWidth, windowHeight);
	sf::RenderWindow window(mode, title);
	std::cout << windowWidth << windowHeight << std::endl;
	// sprite
	sf::Sprite block(blockTexture);
	sf::Vector2i origin(int(fieldWidth / 2), 0);
	sf::Vector2i pos(origin);
	block.setPosition(getPixelPosition(pos, blockSize));
	sf::Clock clock;

	while (window.isOpen()) {
		// action
		enum class Action {
			Hold,
			Left,
			Right,
			Down,
		};
		Action action = Action::Hold;

		// window event
		sf::Event evt;
		if (window.pollEvent(evt)) {
			if (evt.type == evt.Closed) {
				window.close();
			}
			if (evt.type == evt.KeyPressed) {
				if (evt.key.code == sf::Keyboard::Left) {
					action = Action::Left;
				}
				if (evt.key.code == sf::Keyboard::Right) {
					action = Action::Right;
				}	
				if (evt.key.code == sf::Keyboard::Down) {
					action = Action::Down;
				}
			}

		}
		if (clock.getElapsedTime().asSeconds() >= 0.5f) {
			action = Action::Down;
			clock.restart();
		}
		// collision detection 
		sf::Vector2i nextPos = pos; 
		switch (action) {
			case Action::Hold:
				break;
			case Action::Left:
				nextPos.x--;
				break;
			case Action::Right:
				nextPos.x++;
				break;
			case Action::Down:
				nextPos.y++;
				break;
		}
		if (nextPos.x >= 0 && nextPos.x < fieldWidth && nextPos.y < fieldHeight && field[nextPos.x][nextPos.y] == false) {
			pos = nextPos;
			}
		else {
			if (action == Action::Down) { 
				field[pos.x][pos.y] = true;
				// check the pos.y row is full?
				bool isFull = true;
				for (int k = 0; k < fieldWidth; k++) {
					if (field[k][pos.y] == false) { isFull = false; }
				}
				if (isFull) {
					// copy the previous raw (y-1)
					for (int y = pos.y; y > 0; y--) {
						for (int x = 0; x < fieldWidth; x++) {
							field[x][y] = field[x][y - 1];
						}
					}
					// clear top raw
					for (int x = 0; x < fieldWidth; x++) {
						field[x][0] = false;
					}
				}
				pos = origin;
			}
		}
		window.clear();
		block.setPosition(getPixelPosition(pos, blockSize));
		// draw block
		window.draw(block);
		// draw field
		for (int x = 0; x < fieldWidth; x++) {
			for (int y = 0; y < fieldHeight; y++) {
				if (field[x][y] == true) {
					sf::Vector2i p(x, y);
					block.setPosition(getPixelPosition(p, blockSize));
					window.draw(block);
				}
			}
		}
		window.display();
	}
	return EXIT_SUCCESS;
}