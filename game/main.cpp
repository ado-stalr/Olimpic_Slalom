#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <cstdlib>
#include "vectors/main.h"

struct Player
{
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Vector2f position;
};

constexpr unsigned WINDOW_WIDTH = 800;
constexpr unsigned WINDOW_HEIGHT = 600;
const float MOVING_SPEED = 600.0;

// Инициализирует игрока
void initPlayer(Player &player)
{
    player.texture.loadFromFile("textures/sheep.png");
    player.sprite.setTexture(player.texture);
    sf::FloatRect boundingRect = player.sprite.getLocalBounds();
    player.sprite.setOrigin(boundingRect.width / 2, boundingRect.height / 2);
    player.position = {-100, WINDOW_HEIGHT + 20};
    player.sprite.setPosition(player.position);

    player.sprite.setTextureRect(sf::IntRect(10, 10, 500, 500));
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

void update(Player &player, float deltaTime)
{
    const sf::Vector2f playerPosition = player.position;
    const sf::Vector2f delta = playerPosition - sf::Vector2f({0, 0});
    const float moduleDelta = vectorModule(delta.x, delta.y);
    if (!moduleDelta)
        return;

    sf::Vector2f movingFrame = {0, -MOVING_SPEED * deltaTime};
    sf::Vector2f newPosition = playerPosition + movingFrame;
    if (newPosition.y < -100)
    {
        float rand = float(std::rand() % WINDOW_WIDTH / 2) - WINDOW_WIDTH / 4;
        std::cout << rand << std::endl;
        newPosition = {rand, WINDOW_HEIGHT + 500};
    }
    player.position = newPosition;
}
// Рисует и выводит один кадр
void redrawFrame(sf::RenderWindow &window, Player &player)
{
    const float perspKoef = 1 + 0.5 * (player.position.y / WINDOW_HEIGHT);
    player.sprite.setPosition({player.position.x * perspKoef, player.position.y});
    player.sprite.setScale({perspKoef - 0.9, perspKoef - 0.9});
    player.sprite.move({WINDOW_WIDTH / 2, 0});
    window.clear(sf::Color(0xFF, 0xFF, 0xFF));
    window.draw(player.sprite);
    window.display();
}

int main()
{
    sf::Clock clock;
    std::srand(std::time(nullptr));
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(
        sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}),
        "Arrow follows mouse", sf::Style::Default, settings);

    Player player;

    initPlayer(player);

    while (window.isOpen())
    {
        float deltaTime = getDeltaTime(clock);

        pollEvents(window);
        update(player, deltaTime);
        redrawFrame(window, player);
    }
}
