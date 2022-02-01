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

const unsigned WINDOW_WIDTH = 1280;
const unsigned WINDOW_HEIGHT = 800;
const float MOVING_SPEED = 3000.0;
const float INTRO_BALL_SIZE = 40.0;


int viewMode = 0;     // 0 - Начальная заставка
                      // 1 - режим паузы
                      // 2 - режим игры
                      // 3 -
                      // 4 -
                      // 5 - режим ввода имени для сохранения рекорда
                      // 6 - Проигрыш нажми кей
                      // 7 - Победа нажми Key
                      // 8 - Рейтинг
                      // 9 - Титры


// Инициализирует игрока
void initPlayer(Player &player)
{
    player.texture.loadFromFile("textures/pointer.png");
    player.sprite.setTexture(player.texture);
    sf::FloatRect boundingRect = player.sprite.getLocalBounds();
    player.sprite.setOrigin(boundingRect.width / 2, boundingRect.height / 2);
    player.position = {-100, WINDOW_HEIGHT + 20};
    player.sprite.setPosition(player.position);
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
//        case sf::Event::MouseButtonPressed:
//            onMouseClick(event);
//            break;
        default:
            break;
        }
    }
}

void pollEventsIntro(sf::RenderWindow &window)
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
                viewMode = 1;
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
    if (newPosition.y < -20)
    {
        float rand = float(std::rand() % WINDOW_WIDTH / 2) - WINDOW_WIDTH / 4;
        std::cout << rand << std::endl;
        newPosition = {rand, WINDOW_HEIGHT};
    }
    player.position = newPosition;
}

void updateIntro(sf::CircleShape *introBall, float deltaTime, sf::Clock timer)
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

// Рисует и выводит один кадр
void redrawFrame(sf::RenderWindow &window, Player &player)
{
    const float perspKoef = 1 + 0.5 * (WINDOW_HEIGHT / player.position.y);
    player.sprite.setPosition({player.position.x * perspKoef, player.position.y});
    player.sprite.setScale({perspKoef - 0.5, perspKoef - 0.5});
    player.sprite.move({WINDOW_WIDTH / 2, 0});
    window.clear(sf::Color(0xFF, 0xFF, 0xFF));
    window.draw(player.sprite);
    window.display();
}

int main()
{
    sf::Clock clock;
    sf::Clock timer;

    sf::CircleShape introBall(INTRO_BALL_SIZE);
    introBall.setFillColor(sf::Color(0xBF, 0xBF, 0xBF));
    introBall.setPosition(-100, 0);

    std::srand(std::time(nullptr));
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(
        sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}),
        "Olimpic slalom", sf::Style::Default, settings);

    Player player;

    initPlayer(player);

    while (window.isOpen())
    {
        float deltaTime = getDeltaTime(clock);
        switch (viewMode)
        {
            case 0:
                pollEventsIntro(window);
                updateIntro(&introBall, deltaTime, timer);
                redrawFrameIntro(window, timer, introBall);
                break;
//            case 1:
//
//                break;
            default:


                pollEvents(window);
                update(player, deltaTime);
                redrawFrame(window, player);
                break;
        }
    }
}
