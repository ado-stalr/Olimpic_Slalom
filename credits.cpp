#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include "vectors/main.h"

std::vector<std::string> strArr = {"THE END", "", "Idea by creado", "Main dev group: creado", "Production: creado", "Animation: creado", "Music cast: creado", "Created by creado", "IPS, 2022"};

void redrawFrameOutro(sf::RenderWindow &window, sf::Clock timer)
{
    float duration = 30.f;
    sf::Font font;
    font.loadFromFile("arial.ttf");
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(90); // in pixels!
    text.setFillColor(sf::Color(0xDF, 0xDF, 0xDF));
    text.setStyle(sf::Text::Bold);

    window.clear(sf::Color(0, 0, 0));

    float time = timer.getElapsedTime().asSeconds();
    text.move({100.f, 1000.f - time * strArr.size() * 250 / duration});
    if(time < duration + 10)
    {
        for (int i = 0; i < strArr.size(); i++)
        {
            text.setString(strArr[i]);
            window.draw(text);
            text.move({0, 200.f});
        }
    }
}

void pollEventsOutro(int* viewMode, sf::RenderWindow &window)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                switch (event.key.code)
                {
                    case sf::Keyboard::Escape:
                        window.close();
                        break;
                }
               break;
            default:
                break;
        }
    }
}