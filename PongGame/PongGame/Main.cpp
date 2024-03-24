#include <iostream>
#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    sf::CircleShape shape(10.f);
    shape.setFillColor(sf::Color::Green);

    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Time elapsed = clock.getElapsedTime();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        shape.setPosition(sf::Vector2f(40.f, 40.f) + sf::Vector2f(40.f, 40.f) * std::sin(elapsed.asSeconds()));

        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}