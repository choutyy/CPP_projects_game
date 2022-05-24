#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
    std::cout << "Why" << std::endl;
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 600), L"以前念電機系用visual c++ 2005超痛苦");
    
    // Create a graphical text to display
    sf::Font font;
    if (!font.loadFromFile("msjh.ttc"))
        return EXIT_FAILURE;
    sf::Text text(L"Hello 你好", font, 50);
    
    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();
        }
        // Clear screen
        window.clear();
        window.draw(text);
        // Update the window
        window.display();
    }
    return EXIT_SUCCESS;
}