#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>

int main() {	
	// block: Texture and sprite
	sf::Texture blocktexture;
	blocktexture.loadFromFile("block.png");
	sf::Sprite block(blocktexture);
	sf::Vector2f blockSize(block.getLocalBounds().width, block.getLocalBounds().height);
	// size: field, snake, food
	sf::Vector2i fieldSize(40, 30);
	std::vector<sf::Vector2i> snake = { sf::Vector2i (3, 4) };	
	sf::Vector2i food(rand() % fieldSize.x, rand() % fieldSize.y);
	// text
	sf::Font font;
	if (font.loadFromFile("msjh.ttc") == false) { return EXIT_FAILURE; }
	sf::Text text("Eaten Food #:", font);
	sf::Text endText("You Died!\nPresee any key to restart the game", font, 50);
	endText.setPosition(
		unsigned int((fieldSize.x* blockSize.x)/4.0f),
		unsigned int((fieldSize.y* blockSize.y)/2.5f));
	endText.setFillColor(sf::Color::Red);
	// create window
	sf::VideoMode mode( 
		unsigned int (fieldSize.x * blockSize.x),
		unsigned int (fieldSize.y * blockSize.y)
	);
	sf::RenderWindow window(mode, L"貪食蛇");
	// default
	enum class Direction {UP, DOWN, LEFT, RIGHT};
	Direction direction = Direction::RIGHT;	
	bool isDead = false;
	int foodNum = 0;
	sf::Text fN(std::to_string(foodNum), font);
	fN.setPosition(text.getLocalBounds().width + 10.0f, 0);
	//clock
	sf::Clock clock;
	sf::Clock clockGame;

	// window 
	while (window.isOpen()) {
		// window event (all interaction between user and window, including keyPressed)
		sf::Event evt;
		if (window.pollEvent(evt)) {
			if (evt.type == evt.Closed) {
				window.close();
			}
			if (evt.type == evt.KeyPressed) {
				int key = evt.key.code;
				//std::cout << key << std::endl;
				switch (key) {
					case 73:
						direction = Direction::UP;
						break;
					case 74:
						direction = Direction::DOWN;
						break;
					case 71:
						direction = Direction::LEFT;
						break;
					case 72:
						direction = Direction::RIGHT;
						break;
				}
				if (isDead) {
					//	isDead = false;
					std::cout << key << std::endl;;
				}
				// press any key to close the game window
			}
		}

		// object motion
		if (!isDead){
			// motion speed changes over time goes by
			float timeTotal = clockGame.getElapsedTime().asSeconds();
			float speed = 0.08f;
			//if (timeTotal >= 30.f && timeTotal <= 50.0f) { speed = 0.1f; }
			//else if (timeTotal > 50.0f && timeTotal <= 70.0f) { speed = 0.06f; }
			//else if (timeTotal > 70.0f) { speed = 0.02f; }
			// 
			if (clock.getElapsedTime().asSeconds() >= speed)
			{
				sf::Vector2i head = snake[0];
				if (direction == Direction::UP) { head.y--; }
				if (direction == Direction::DOWN) { head.y++; }
				if (direction == Direction::LEFT) { head.x--; }
				if (direction == Direction::RIGHT) { head.x++; }

				// snake body 
				snake.insert(snake.begin(), head);
				if (food == head) {
					foodNum++;
					fN.setString(std::to_string(foodNum));
					// receate food position
					food.x = rand() % fieldSize.x;
					food.y = rand() % fieldSize.y;
				}
				else {
					snake.pop_back();
				}
				// snake head is out of side?
				if (head.x <= 0 || head.x >= fieldSize.x || head.y <= 0 || head.y >= fieldSize.y) { isDead = true; }
				// snake head hit its body?
				for (int sI = 1; sI < snake.size(); sI++) {
					sf::Vector2i sbody = snake[sI];
					if (head == sbody) { isDead = true; }
				}
				clock.restart(); // restart the object motion clock(frameRate)
			}
		}		
		// window display
		if (isDead) { 
			window.clear(sf::Color(220, 220, 220));
			window.draw(endText);
		}
		else
		{
			window.clear();
		}		
		// draw snake
		for (const sf::Vector2i& body : snake) {
			sf::Vector2f bodypos(
				body.x * blockSize.x, 
				body.y * blockSize.y); 
			block.setPosition(bodypos);
			window.draw(block);
		} // end for
		// draw food
		sf::Vector2f foodpos(food.x* blockSize.x, food.y* blockSize.y);
		block.setPosition(foodpos);
		window.draw(block);
		// draw text
		window.draw(text); 
		window.draw(fN); //food number
		window.display();
	}
	return EXIT_SUCCESS;
}