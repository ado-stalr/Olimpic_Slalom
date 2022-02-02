#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <iostream>
#include <cstdlib>
#include "vectors/main.h"

// Опрашивает и обрабатывает доступные события в цикле
void pollEventsWin(int* viewMode, sf::RenderWindow &window, sf::Music *winSound)
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
                //restart
                winSound->stop();
            default:
                break;
        }
    }
}