#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <iostream>
#include <cstdlib>
#include "vectors/main.h"
#include "intro.cpp"
#include "credits.cpp"

struct Player
{
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Vector2f position;
};
struct Snow
{
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Vector2f position;
};

const unsigned WINDOW_WIDTH = 1280;
const unsigned WINDOW_HEIGHT = 800;
const float MOVING_SPEED = 1000.0;
const float INTRO_BALL_SIZE = 40.0;

const std::string LOSE_TEXT_HEADER = "LOSE";
const std::string LOSE_TEXT = "Press key to try again!";
const std::string WIN_TEXT_HEADER = "WIN";
const std::string WIN_TEXT = "Press key to start next race";

int viewMode = 7;     // 0 - Начальная заставка
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
    player.texture.loadFromFile("textures/red_pointer.png");
    player.sprite.setTexture(player.texture);
    sf::FloatRect boundingRect = player.sprite.getLocalBounds();
    player.sprite.setOrigin(boundingRect.width / 2, boundingRect.height / 2);
    player.position = {-100, WINDOW_HEIGHT + 20};
    player.sprite.setPosition(player.position);
}

// Инициализирует снег
void initSnow(std::vector<Snow*> snowArr)
{
    for(int i = 0; i < snowArr.size(); i++)
    {
        snowArr[i]->texture.loadFromFile("textures/white_pointer.png");
        snowArr[i]->sprite.setTexture(snowArr[i]->texture);
        sf::FloatRect boundingRect = snowArr[i]->sprite.getLocalBounds();
        snowArr[i]->sprite.setOrigin(boundingRect.width / 2, boundingRect.height / 2);
        snowArr[i]->position = {float(std::rand() % WINDOW_WIDTH / 2) - WINDOW_WIDTH / 4, -float(std::rand() % WINDOW_HEIGHT)};
        std::cout << float(std::rand() % WINDOW_WIDTH / 2) << ' ';
        snowArr[i]->sprite.setPosition(snowArr[i]->position);
    }
}

// Переводит радианы в градусы
float toDegrees(float radians)
{
    return float(double(radians) * 180.0 / M_PI);
}

float getDeltaTime(sf::Clock &clock)
{
    return clock.restart().asSeconds();
}

// Опрашивает и обрабатывает доступные события в цикле
void pollEventsLose(int* viewMode, sf::RenderWindow &window, sf::Music *lose_sound)
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
                lose_sound->stop();
            default:
                break;
        }
    }
}

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
        default:
            break;
        }
    }
}

void updateSnow(std::vector<Snow*> snowArr, float deltaTime)
{
    for(int i = 0; i < snowArr.size(); i++)
    {
        const sf::Vector2f snowPosition = snowArr[i]->position;
        sf::Vector2f movingFrame = {0, MOVING_SPEED * deltaTime};
        sf::Vector2f newPosition = snowPosition + movingFrame;
        if (newPosition.y > WINDOW_HEIGHT || newPosition.y < -100)
        {
            float rand = float(std::rand() % WINDOW_WIDTH / 2) - WINDOW_WIDTH / 4;
            newPosition = {rand, -float(std::rand() % WINDOW_HEIGHT)};
        }
        snowArr[i]->position = newPosition;
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
        newPosition = {rand, WINDOW_HEIGHT};
    }
    player.position = newPosition;
}

void redrawSnowFrame(sf::RenderWindow &window, std::vector<Snow*> snowArr)
{
    for(int i = 0; i < snowArr.size(); i++)
    {
        const float perspKoef = (snowArr[i]->position.y / WINDOW_HEIGHT * 5);
        snowArr[i]->sprite.setPosition({snowArr[i]->position.x * perspKoef * 5, snowArr[i]->position.y});
        snowArr[i]->sprite.setScale({perspKoef, perspKoef});
        snowArr[i]->sprite.move({WINDOW_WIDTH / 2, 0});
        window.draw(snowArr[i]->sprite);
    }
}

// Рисует и выводит один кадр
void redrawFrameLoseWin(sf::RenderWindow &window, std::vector<Snow*> snowArr, std::string textStr1, std::string textStr2)
{
    window.clear(sf::Color(0xFF, 0xFF, 0xFF));
    redrawSnowFrame(window, snowArr);
    sf::Vector2f pos = {100, 100};

    sf::RectangleShape bg(sf::Vector2f(WINDOW_WIDTH - 200, WINDOW_HEIGHT - 200));
    if(viewMode == 6)
        bg.setFillColor(sf::Color(0xFF, 0xD3, 0x99, 0x44));
    else
        bg.setFillColor(sf::Color(0xD5, 0xFF, 0x99, 0x44));

    bg.setPosition(pos);

    sf::Text text;
    sf::Font font;
    font.loadFromFile("arial.ttf");
    text.setFont(font);
    text.setString(textStr1);
    text.setCharacterSize(100);
    if(viewMode == 6)
        text.setFillColor(sf::Color(0xDF, 0x0, 0x0));
    else
        text.setFillColor(sf::Color(0x0, 0xDF, 0x0));
    text.setStyle(sf::Text::Bold);
    text.setPosition({pos.x + 100, pos.y + 100});

    window.draw(bg);
    window.draw(text);

    text.setString(textStr2);
    text.setCharacterSize(70);
    text.setFillColor(sf::Color(0x0, 0x0, 0x0));
    text.setStyle(sf::Text::Bold);
    text.setPosition({pos.x + 100, pos.y + 400});

    window.draw(text);
    window.display();
}

// Рисует и выводит один кадр
void redrawFrame(sf::RenderWindow &window, Player &player, std::vector<Snow*> snowArr)
{
    window.clear(sf::Color(0xFF, 0xFF, 0xFF));
    redrawSnowFrame(window, snowArr);
    window.display();
}

int main()
{
    sf::Clock clock;
    sf::Clock timer;
    sf::Music introMusic;
    introMusic.openFromFile("sounds/intro_music.ogg");
    introMusic.setLoop(true);
    sf::Music winMusic;
    winMusic.openFromFile("sounds/win_sound.ogg");
    sf::Music loseMusic;
    loseMusic.openFromFile("sounds/lose_sound.ogg");
//    introMusic.play();

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
    std::vector<Snow*> snowArr;
    snowArr.push_back(new Snow);
    snowArr.push_back(new Snow);
    snowArr.push_back(new Snow);
    snowArr.push_back(new Snow);

    initPlayer(player);
    initSnow(snowArr);

    while (window.isOpen())
    {
        float deltaTime = getDeltaTime(clock);
        switch (viewMode)
        {
            case 0:
                pollEventsIntro(&viewMode, window, &introMusic);
                updateIntro(&viewMode, &introBall, deltaTime, timer, &introMusic);
                redrawFrameIntro(window, timer, introBall);
                break;
            case 6:
                pollEventsLose(&viewMode, window, &loseMusic);
                updateSnow(snowArr, deltaTime);
                redrawFrameLoseWin(window, snowArr, LOSE_TEXT_HEADER, LOSE_TEXT);
                break;
            case 7:
                pollEventsWin(&viewMode, window, &winMusic);
                updateSnow(snowArr, deltaTime);
                redrawFrameLoseWin(window, snowArr, WIN_TEXT_HEADER, WIN_TEXT);
                break;
            case 9:
                pollEventsOutro(&viewMode, window);
                redrawFrameOutro(window, timer, &introMusic);
                break;
            default:
                pollEvents(window);
                updateSnow(snowArr, deltaTime);
                update(player, deltaTime);
                redrawFrame(window, player, snowArr);
                break;
        }
    }
}
