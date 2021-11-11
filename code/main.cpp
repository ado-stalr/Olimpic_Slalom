#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <cstdint>
#include <iostream>


// Инициализирует игрока
void initPlayer(sf::Sprite &player, sf::Texture &playerTexture)
{
    playerTexture.loadFromFile("code/textures/player.png");
    player.setTexture(playerTexture);
    sf::FloatRect boundingRect = player.getLocalBounds();
    player.setOrigin(boundingRect.width / 2, boundingRect.height / 2);
    player.setPosition(400, 300);
}

// Переводит радианы в градусы
float toDegrees(float radians)
{
    return float(double(radians) * 180.0 / M_PI);
}

// Обрабатывается событие MouseMove, обновляя позицию мыши
void onMouseClick(const sf::Event &event)
{
    if (event.mouseButton.button == sf::Mouse::Left)
    {
        
        std::cout << "the left button was pressed" << std::endl;
        std::cout << "mouse x: " << event.mouseButton.x << std::endl;
        std::cout << "mouse y: " << event.mouseButton.y << std::endl;
    }
}

float getDeltaTime(sf::Clock &clock)
{
    return clock.restart().asSeconds();
}

// Опрашивает и обрабатывает доступные события в цикле
void pollEvents(sf::RenderWindow &window)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed:
            window.close();
            break;
        case sf::Event::MouseButtonPressed:
            onMouseClick(event);
            break;
        default:
            break;
        }
    }
}

float vectorModule(float x, float y)
{
    return sqrt((x * x) + (y * y));
}

void update(sf::Sprite &player, float deltaTime)
{
    const sf::Vector2f playerPosition = player.getPosition();
    const sf::Vector2f delta = pointerPosition - {0,0};
    const float moduleDelta = moduleVectora(delta.x, delta.y);
    if (!moduleDelta)
        return;

// Рисует и выводит один кадр
void redrawFrame(sf::RenderWindow &window, sf::Sprite &player)
{
    window.clear(sf::Color(0xFF, 0xFF, 0xFF));
    window.draw(player);
    window.display();
}

int main()
{
    constexpr unsigned WINDOW_WIDTH = 800;
    constexpr unsigned WINDOW_HEIGHT = 600;
    sf::Clock clock;
    
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(
        sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}),
        "Arrow follows mouse", sf::Style::Default, settings);

    sf::Sprite player;
    sf::Texture playerTexture;

    initPlayer(player, playerTexture);

    while (window.isOpen())
    {
        float deltaTime = getDeltaTime(clock);

        pollEvents(window);
        update(pointer, player, deltaTime);
        redrawFrame(window, player);
    }
} 
