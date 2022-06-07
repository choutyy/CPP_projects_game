#include <SFML/Graphics.hpp>
int main() {
	const int fieldWidth = 10;
	const int fieldHeight = 20;
	enum class Type {
		None,
		O,
		I,
	};
	Type field[fieldWidth][fieldHeight] = {}; // 0 indicates none
	// Texture
	sf::Texture backgroundTexture;
	if (!backgroundTexture.loadFromFile("background.png")) {
		return EXIT_FAILURE;
	}
	sf::Texture yellowTexture;
	if (!yellowTexture.loadFromFile("yellow.png")) {
		return EXIT_FAILURE;
	}
	sf::Texture lightBlueTexture;
	if (!lightBlueTexture.loadFromFile("light_blue.png")) {
		return EXIT_FAILURE;
	}
	// shapes
	std::map<Type, std::vector<std::vector<sf::Vector2i>>> shapes = {
		{
			Type::O,
			{
				// O:     OO    (0,-1) (1,-1)
				//        OO    (0, 0) (1, 0)
				{
					sf::Vector2i(0,0),
					sf::Vector2i(1,0),
					sf::Vector2i(0,-1),
					sf::Vector2i(1,-1),
				}
			},
		},
		{
			Type::I,
			{
				// I:     OOOO   (-1, 0) (0, 0) (1,0) (2,0)  shape_Is[0]
				{
					sf::Vector2i(-1,0),
					sf::Vector2i(0,0),
					sf::Vector2i(1,0),
					sf::Vector2i(2,0),
				},
				// I:     O   (0,-2)                         shape_Is[1]
				//        O   (0,-1)
				//        O   (0, 0)
				//        O   (0, 1)
				{
					sf::Vector2i(0,-2),
					sf::Vector2i(0,-1),
					sf::Vector2i(0,0),
					sf::Vector2i(0,1),
				}
			}
		}
	};
	
	// Window
	const int blockWidth = yellowTexture.getSize().x;
	const int blockHeight = yellowTexture.getSize().y;
	const int windowWidth = fieldWidth * blockWidth;
	const int windowHeight = fieldHeight * blockHeight;
	sf::RenderWindow win(sf::VideoMode(windowWidth, windowHeight), L"俄羅斯方塊");

	// sprites
	std::map<Type, sf::Sprite> sprites = {
		{Type::O, sf::Sprite(yellowTexture)},
		{Type::I, sf::Sprite(lightBlueTexture)}
	};
	backgroundTexture.setRepeated(true);
	sf::Sprite backgroundSprite(
		backgroundTexture,
		sf::IntRect(0, 0, windowWidth, windowHeight));

	// default values
	sf::Clock clock;
	std::vector<sf::Vector2i> currentShape;
	sf::Sprite currentSprite;
	const sf::Vector2i origin(fieldWidth / 2, 0);
	sf::Vector2i pos(origin);
	Type currentType = Type(rand() % 2 + 1);
	int currentIndex = 0; // rotation


	while (win.isOpen()) {
		currentShape = shapes[currentType][currentIndex];
		currentSprite = sprites[currentType];
		enum class Action {
			Hold,
			Left,
			Right,
			Down,
			Rotate,
		};
		Action action = Action::Hold;

		sf::Event evt;
		if (win.pollEvent(evt)) {
			if (evt.type == evt.Closed) {
				win.close();
			}
			if (evt.type == evt.KeyPressed) {
				switch (evt.key.code) {
					case sf::Keyboard::Left:
						action = Action::Left;
						break;
					case sf::Keyboard::Right:
						action = Action::Right;
						break;
					case sf::Keyboard::Up:
						action = Action::Rotate;
						break;
				}
			}
		}
		if (clock.getElapsedTime().asSeconds() > 0.2f) {
			action = Action::Down;
			clock.restart();
		}
		// After user finishing enter commands, checl nextpos valid or not 
		sf::Vector2i nextPos = pos;
		int nextIndex = currentIndex;
		if (action == Action::Rotate) {
			nextIndex = (nextIndex + 1) % shapes[currentType].size(); // shapes_Os: 1; shapes_Is: 2
		}
		std::vector<sf::Vector2i> nextShape = shapes[currentType][nextIndex];
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
		//bool isEmpty = false;
		int countEmpty = 0; 
		for (const sf::Vector2i& d : nextShape) {
			sf::Vector2i np = nextPos + d;
			if (np.x >= 0 && np.x < fieldWidth && 
				np.y < fieldHeight && 
				(np.y < 0 || field[np.x][np.y] == Type::None)) { // check this logic: np.y < 0
				//isEmpty = true; // 沒問題才assign給pos, isEmpty = true
				countEmpty++;
			}
		}
		
		if (countEmpty == 4) {
			pos = nextPos; // 沒問題才assign給pos, isEmpty = true
			currentIndex = nextIndex;
			currentShape = nextShape;
		}
		else { // 有問題就不assign給pos，表示上一個pos已經是極限
			if (action == Action::Down) {
				for (const sf::Vector2i& d : currentShape) {
					sf::Vector2i np = pos + d;
					if (np.y > 0) { // np.y > 0 表示在場地範圍內
						field[np.x][np.y] = currentType; 
					} 
				}	
				// 檢查是否排滿
				for (int y = 0; y < fieldHeight; y++) {
					bool isFull = true;
					for (int x = 0; x < fieldWidth; x++) {
						if (field[x][y] == Type::None) {
							isFull = false;
						}
					}
					if (isFull) {
						for (int currY = y; currY > 0; currY--) {
							for (int x = 0; x < fieldWidth; x++) {
								field[x][currY] = field[x][currY - 1]; // copy 上面一行
							}
						}
						for (int x = 0; x < fieldWidth; x++) {
							field[x][0] = Type::None;
						}
					}					
				}
				// renew 
				pos = origin;
				currentType = Type(rand() % 2 + 1);
				currentIndex = 0;
			}
		}		

		win.clear();
		// draw background
		win.draw(backgroundSprite);
		// draw field
		for (int x = 0; x < fieldWidth; x++) {
			for (int y = 0; y < fieldHeight; y++) {
				if (field[x][y] == Type::None) continue;

				sf::Sprite& s = sprites[field[x][y]];
				s.setPosition(
					float(x* blockWidth),
					float(y* blockHeight)
				);
				win.draw(s);				
			}
		}
		// draw block
		for (const sf::Vector2i& d : currentShape) {
			sf::Vector2i np = pos + d; 
			currentSprite.setPosition(
				float(np.x* blockWidth),
				float(np.y* blockHeight)
			);
			win.draw(currentSprite);
		}		
		
		win.display();
	}

	return EXIT_SUCCESS;
}