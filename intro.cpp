#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <iostream>
#include <cstdlib>
#include "vectors/main.h"

void updateIntro(int* viewMode, sf::CircleShape *introBall, float deltaTime, sf::Clock timer, sf::Music *introMusic)
{
    const sf::Vector2f position = {-100, 0};
    constexpr float speedX = 200.f;
    constexpr float amplitudeY = 280.f;
    constexpr float periodY = 2;

    const float time = timer.getElapsedTime().asSeconds();
    const float wavePhase = time * float(2 * M_PI);
    const float x = speedX * time;
    const float y = amplitudeY * -std::abs(std::sin(wavePhase / periodY)) + speedX * time / 3;
    const sf::Vector2f offset = {x, y};
    introBall->setPosition(position + offset);

    if (timer.getElapsedTime().asSeconds() > 15)
    {
        *viewMode = 1;
        introMusic->stop();
    }
}

// Рисует и выводит один кадр
void redrawFrameIntro(sf::RenderWindow &window, sf::Clock timer, sf::CircleShape ball)
{
    std::vector<std::string> charArr = {"c", "r", "e", "a", "d", "o"};
    sf::Font font;
    font.loadFromFile("arial.ttf");
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(90); // in pixels!
    text.setFillColor(sf::Color(0xDF, 0xDF, 0xDF));
    text.setStyle(sf::Text::Bold);
    text.move({100.f, 120.f});

    window.clear(sf::Color(0, 0, 0));
    window.draw(ball);

    for(int i = 0; i < charArr.size(); i++) {
        text.setString(charArr[i]);
        if(timer.getElapsedTime().asSeconds() >= i + 1)
        {
            window.draw(text);
            text.move({200.f, 77.f});
        }
    }

    if(timer.getElapsedTime().asSeconds() >= 10)
    {
        text.setString( "production");
        text.setPosition({200.f, 620.f});
        text.setCharacterSize(70); // in pixels!
        text.setFillColor(sf::Color(0x77, 0x77, 0x77));
        window.draw(text);
    }

    window.display();
}



void pollEventsIntro(int* viewMode, sf::RenderWindow &window, sf::Music *intro_sound)
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
                *viewMode = 1;
                intro_sound->stop();
                break;
            default:
                break;
        }
    }
}