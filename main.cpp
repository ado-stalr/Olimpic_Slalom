#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include "vectors/main.h"
#include "types.cpp"
#include "intro.cpp"
#include "credits.cpp"
#include "levels.cpp"

#define PI 3.14159265

const unsigned WINDOW_WIDTH = 1280;
const unsigned WINDOW_HEIGHT = 800;
const float MOVING_SPEED = 1000.0;
const float INTRO_BALL_SIZE = 40.0;
std::string POINTS_TEXT = "Checkpoints: ";
std::string TIME_TEXT = "Time: ";

struct Snow
{
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Vector2f position;
};

const std::string GAME_TITLE = "OLIMPIC SLALOM";
const std::string LOSE_TEXT_HEADER = "LOSE";
const std::string LOSE_TEXT = "Press key to try again!";
const std::string WIN_TEXT_HEADER = "WIN";
const std::string WIN_TEXT = "Press key to start next race";
const std::string START_WINDOW_TEXT = "Press Enter to start game";
const std::string PAUSE_TEXT = "Press Enter to resume game";

int viewMode = 0;     // 0 - Начальная заставка
                      // 1 - режим паузы
                      // 2 - режим игры
                      // 3 -
                      // 4 -
                      // 5 - режим ввода имени для сохранения рекорда     TODO
                      // 6 - Проигрыш нажми кей
                      // 7 - Победа нажми Key
                      // 8 - Рейтинг        TODO
                      // 9 - Титры

float getRandomXposition()
{
    float randomNumber = float(std::rand() % ((WINDOW_WIDTH - CHECKPOINT_WIDTH))) - (WINDOW_WIDTH - CHECKPOINT_WIDTH)/ 2;
    return randomNumber;
}

void stopAllSounds(Game *game)
{
    game->sounds.introSound.stop();
    game->sounds.gameSound.stop();
    game->sounds.outroSound.stop();
    game->sounds.winSound.stop();
    game->sounds.loseSound.stop();
}

void checkForUpdateLevel(Game *game, int *viewMode)
{
    if(game->points >= game->levels[game->level].checkpoints && game->time <= game->levels[game->level].time)
    {
        *viewMode = 7;
        stopAllSounds(game);
        game->sounds.winSound.play();
    }
    else
    {
        *viewMode = 6;
        stopAllSounds(game);
        game->sounds.loseSound.play();
    }
}

void updateLevel(Game *game, int *viewMode)
{
    if(game->points >= game->levels[game->level].checkpoints && game->time <= game->levels[game->level].time)
    {
        game->level++;
        *viewMode = 2;
        stopAllSounds(game);
        game->sounds.gameSound.play();
    }
    if(game->level + 1 == game->levels.size())
    {
        *viewMode = 9;
        stopAllSounds(game);
        game->sounds.outroSound.play();
    }
}

void initObjectPositions(Game *game)
{
    game->player.position = {0 , 100};
    game->player.angle = 90.f;
    game->player.sprite.setPosition(game->player.position);
    game->player.sprite.setRotation(game->player.angle);

    game->start.position = {0, 1000};
    game->start.shape.setFillColor(sf::Color(0xFF, 0, 0));
    game->start.shape.setOrigin(WINDOW_WIDTH / 2, 0);
    game->start.shape.setSize({WINDOW_WIDTH, game->start.height});

    game->finish.position = {0, float(game->levels[game->level].distance) + 1000.f};

    for (int i = 0; i < game->checkpointsArr.size(); i++)
    {
        float randomNumber = getRandomXposition();
        game->checkpointsArr[i]->position = {randomNumber, 1500.f + i * 500};
        game->checkpointsArr[i]->isTouched = false;

    }
    game->points = 0;
    game->time = 0;
    game->isStarted = false;
    game->isLeftPressed = false;
    game->isRightPressed = false;
}

void initGame(Game *game)
{
    game->background.texture.loadFromFile("textures/background.png");
    game->background.sprite.setTexture(game->background.texture);
    game->background.position = {0 , 0};
    game->background.sprite.setPosition(game->player.position);

    game->player.size = 200;
    game->player.texture.loadFromFile("textures/player_2.png");
    game->player.sprite.setTexture(game->player.texture);
    sf::FloatRect boundingRect = game->player.sprite.getLocalBounds();
    game->player.sprite.setOrigin(boundingRect.width / 2, boundingRect.height / 2);
    game->player.position = {0 , 100};
    game->player.angle = 90.f;
    game->player.sprite.setPosition(game->player.position);
    game->player.sprite.setRotation(game->player.angle);

    game->start.position = {0, 1000};
    game->start.shape.setFillColor(sf::Color(0xFF, 0, 0));
    game->start.shape.setOrigin(WINDOW_WIDTH / 2, 0);
    game->start.shape.setSize({WINDOW_WIDTH, game->start.height});

    game->finish.texture.loadFromFile("textures/finish.png");
    game->finish.sprite.setTexture(game->finish.texture);
    sf::FloatRect boundingRectFinish = game->finish.sprite.getLocalBounds();
    game->finish.sprite.setOrigin(boundingRectFinish.width / 2, 0);
    game->finish.position = {0, 2000};
    game->finish.sprite.setPosition(game->finish.position);

    for (int i = 0; i < game->checkpointsArr.size(); i++)
    {
        game->checkpointsArr[i]->shape.setSize({game->checkpointsArr[i]->size.x, game->checkpointsArr[i]->size.y});
        game->checkpointsArr[i]->shape.setFillColor(sf::Color(0xFF, 0, 0));
        game->checkpointsArr[i]->shape.setOrigin(game->checkpointsArr[i]->size.x / 2, 0);
        game->checkpointsArr[i]->position = {getRandomXposition(), 1500.f + i * 500};
    }

    game->sounds.introSound.openFromFile("sounds/intro_sound.ogg");
    game->sounds.gameSound.openFromFile("sounds/game_sound.ogg");
    game->sounds.gameSound.setLoop(true);
    game->sounds.outroSound.openFromFile("sounds/outro_sound.ogg");
    game->sounds.outroSound.setLoop(true);
    game->sounds.winSound.openFromFile("sounds/win_sound.ogg");
    game->sounds.loseSound.openFromFile("sounds/lose_sound.ogg");
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
        snowArr[i]->position = {float(std::rand() % WINDOW_WIDTH) - WINDOW_WIDTH / 2, -float(std::rand() % 2 * WINDOW_HEIGHT)};
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
void pollEventsLose(int* viewMode, sf::RenderWindow &window, Game *game)
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
                initObjectPositions(game);
                *viewMode = 2;
                stopAllSounds(game);
                game->sounds.gameSound.play();
            default:
                break;
        }
    }
}

// Опрашивает и обрабатывает доступные события в цикле
void pollEventsMenu(int* viewMode, sf::RenderWindow &window, Game *game)
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
                    case sf::Keyboard::Enter:
                    case sf::Keyboard::Space:
                        *viewMode = 2;
                        break;
                }
            default:
                break;
        }
    }
}

// Опрашивает и обрабатывает доступные события в цикле
void pollEventsWin(int* viewMode, sf::RenderWindow &window, Game *game, sf::Clock *timer)
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
                timer->restart();
                updateLevel(game, viewMode);
                initObjectPositions(game);
                game->sounds.winSound.stop();
            default:
                break;
        }
    }
}

// Опрашивает и обрабатывает доступные события в цикле
void pollEventsGame(int *viewMode, sf::RenderWindow &window, Game *game)
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
                game->isRightPressed = false;
                game->isLeftPressed = false;
                *viewMode = 1;
                break;
            case sf::Keyboard::Left:
                game->isLeftPressed = true;
                break;
            case sf::Keyboard::Right:
                game->isRightPressed = true;
                break;
            }
            break;
        case sf::Event::KeyReleased:
            switch (event.key.code)
            {
                case sf::Keyboard::Left:
                    game->isLeftPressed = false;
                    break;
                case sf::Keyboard::Right:
                    game->isRightPressed = false;
                    break;
            }
            break;
        default:
            break;
        }
    }
}

const float KOEF_CORRECTION = 0.6;
float calculatePerspKoef(float y)
{
    return 0.3 * (y / WINDOW_HEIGHT) + 1 - KOEF_CORRECTION;
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

void updateGame(int *viewMode, Game *game, float deltaTime, sf::Clock *timer)
{
    if(game->isRightPressed)
    {
        game->player.angle = std::max(game->player.angle - deltaTime * game->rotationSpeed, 1.f);
    }
    if(game->isLeftPressed)
    {
        game->player.angle = std::min(game->player.angle + deltaTime * game->rotationSpeed, 179.f);
    }
    float deltaX = std::cos(game->player.angle * PI / 180) * game->movingSpeed * deltaTime;
    float deltaY = std::sin(game->player.angle * PI / 180) * game->movingSpeed * deltaTime;

    game->player.position.x += deltaX;

    float perspKoef = calculatePerspKoef(game->player.position.y);
    if((game->player.position.x + game->player.size * perspKoef >= 0.5 * WINDOW_WIDTH) || (game->player.position.x - game->player.size * perspKoef <= -0.5 * WINDOW_WIDTH))
    {
        *viewMode = 6;
        stopAllSounds(game);
        game->sounds.loseSound.play();
        return;
    }

    game->start.position.y -= deltaY;
    game->finish.position.y -= deltaY;

    if(game->isStarted) {
        game->time = timer->getElapsedTime().asMilliseconds() / 1000;
    }
    if(!game->isStarted && game->player.position.y >= game->start.position.y)   // collision with start
    {
        game->isStarted = true;
        timer->restart();
    }
    if(game->isStarted && game->player.position.y >= game->finish.position.y)   // collision with finish
    {
        checkForUpdateLevel(game, viewMode);
    }

    for(int i = 0; i < game->checkpointsArr.size(); i++)  // collision with checkpoints
    {
        game->checkpointsArr[i]->position.y -= deltaY;
        if(!game->checkpointsArr[i]->isTouched
           && game->player.position.y >= game->checkpointsArr[i]->position.y
           && game->player.position.y - game->player.sprite.getLocalBounds().height / 2 <= game->checkpointsArr[i]->position.y
           && game->player.position.x <= game->checkpointsArr[i]->position.x + game->checkpointsArr[i]->size.x / 2
           && game->player.position.x >= game->checkpointsArr[i]->position.x - game->checkpointsArr[i]->size.x / 2)
        {
            game->checkpointsArr[i]->isTouched = true;
            game->points ++;
        }
        if(game->checkpointsArr[i]->position.y <= -10)
        {
            game->checkpointsArr[i]->isTouched = false;
            game->checkpointsArr[i]->position = {getRandomXposition(), 1000.f};
        }
    }

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

void renderPointsText(sf::RenderWindow &window, Game *game, std::string text, sf::Vector2f pos)
{
    sf::Text pointsText;
    sf::Font font;
    font.loadFromFile("arial.ttf");
    pointsText.setFont(font);
    pointsText.setString(text + std::to_string(game->points) + "/" + std::to_string(game->levels[game->level].checkpoints));
    pointsText.setCharacterSize(36);
    pointsText.setFillColor(sf::Color(0x99, 0x99, 0x99));
    pointsText.setStyle(sf::Text::Bold);
    pointsText.setPosition(pos);

    window.draw(pointsText);
}

void renderTimeText(sf::RenderWindow &window, Game *game, std::string text, sf::Vector2f pos)
{
    sf::Text timeText;
    sf::Font font;
    font.loadFromFile("arial.ttf");
    timeText.setFont(font);
    timeText.setString(text + std::to_string(game->time) + "s/" + std::to_string(game->levels[game->level].time) + "s");
    timeText.setCharacterSize(36);
    timeText.setFillColor(sf::Color(0x99, 0x99, 0x99));
    timeText.setStyle(sf::Text::Bold);
    timeText.setPosition(pos);

    window.draw(timeText);
}

void renderLevelText(sf::RenderWindow &window, Game *game, std::string text, sf::Vector2f pos, bool isFloatRight = false)
{
    sf::Text levelText;
    sf::Font font;
    font.loadFromFile("arial.ttf");
    levelText.setFont(font);
    levelText.setString(text);
    levelText.setCharacterSize(36);
    levelText.setFillColor(sf::Color(0x66, 0x66, 0x66));
    levelText.setStyle(sf::Text::Bold);
    if (isFloatRight)
    {
        sf::FloatRect boundingRect = levelText.getLocalBounds();
        levelText.setOrigin(boundingRect.width, 0);
    }
    levelText.setPosition(pos);

    window.draw(levelText);
}

void redrawFrameGame(sf::RenderWindow &window, Game *game, std::vector<Snow*> snowArr, bool isPreview)
{
    window.clear(sf::Color(0xFF, 0xFF, 0xFF));

    float startPerspKoef = calculatePerspKoef(game->start.position.y);
    game->start.shape.setPosition({ game->start.position.x * startPerspKoef,  game->start.position.y});
    game->start.shape.setScale({startPerspKoef, startPerspKoef});
    game->start.shape.move({WINDOW_WIDTH / 2, 0});
    window.draw( game->start.shape); // start

    float finishPerspKoef = calculatePerspKoef(game->finish.position.y);
    game->finish.sprite.setPosition({ game->finish.position.x * finishPerspKoef,  game->finish.position.y});
    game->finish.sprite.setScale({finishPerspKoef, finishPerspKoef});
    game->finish.sprite.move({WINDOW_WIDTH / 2, 0});
    window.draw( game->finish.sprite); // finish

    for(int i = 0; i < game->checkpointsArr.size(); i++)
    {
        float checkpointPerspKoef = calculatePerspKoef(game->checkpointsArr[i]->position.y);
        game->checkpointsArr[i]->shape.setPosition({ game->checkpointsArr[i]->position.x * checkpointPerspKoef,  game->checkpointsArr[i]->position.y});
        game->checkpointsArr[i]->shape.setScale({checkpointPerspKoef, checkpointPerspKoef});
        game->checkpointsArr[i]->shape.move({WINDOW_WIDTH / 2, 0});
        if(game->checkpointsArr[i]->isTouched)
            game->checkpointsArr[i]->shape.setFillColor(sf::Color(0, 0xFF, 0));
        else
            game->checkpointsArr[i]->shape.setFillColor(sf::Color(0xFF, 0, 0));
        window.draw( game->checkpointsArr[i]->shape); //checkpoint
    }

    window.draw(game->background.sprite); //background

    float playerPerspKoef = calculatePerspKoef(game->player.position.y);
    game->player.sprite.setPosition({game->player.position.x * playerPerspKoef, game->player.position.y});
    game->player.sprite.setRotation(game->player.angle);
    game->player.sprite.setScale({playerPerspKoef, playerPerspKoef});
    game->player.sprite.move({WINDOW_WIDTH / 2, 0});
    window.draw(game->player.sprite); //player

    redrawSnowFrame(window, snowArr); //snow
    if (!isPreview) {
        renderLevelText(window, game, game->levels[game->level].name, {WINDOW_WIDTH - 20, 20}, true); //level name
        renderPointsText(window, game, POINTS_TEXT, {20, 20});   //points
        renderTimeText(window, game, TIME_TEXT, {20, 70});   //time
    }
}

void redrawFrameLoseWin(sf::RenderWindow &window, std::vector<Snow*> snowArr, Game *game, std::string textStr1, std::string textStr2)
{
    window.clear(sf::Color(0xFF, 0xFF, 0xFF));
    redrawFrameGame(window, game, snowArr, true);
    sf::Vector2f pos = {100, 100};

    sf::RectangleShape bg(sf::Vector2f(WINDOW_WIDTH - 200, WINDOW_HEIGHT - 200));
    if(viewMode == 6)
        bg.setFillColor(sf::Color(0xFF, 0xD3, 0x99, 0xD9));
    else
        bg.setFillColor(sf::Color(0xD5, 0xFF, 0x99, 0xD9));

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

    renderLevelText(window, game, game->levels[game->level].name, {WINDOW_WIDTH / 2, pos.y + 120});
    renderPointsText(window, game, POINTS_TEXT, {WINDOW_WIDTH / 2, pos.y + 200});
    renderTimeText(window, game, TIME_TEXT, {WINDOW_WIDTH / 2, pos.y + 280});
}

void redrawFrameMenu(sf::RenderWindow &window, std::vector<Snow*> snowArr, Game *game, std::string textStr)
{
    window.clear(sf::Color(0xFF, 0xFF, 0xFF));
    redrawFrameGame(window, game, snowArr, true);
    sf::Vector2f pos = {100, 100};

    sf::RectangleShape bg(sf::Vector2f(WINDOW_WIDTH - 200, WINDOW_HEIGHT - 200));
    bg.setFillColor(sf::Color(0xF0, 0xF1, 0xF4, 0xD9));
    bg.setPosition(pos);

    sf::Text text;
    sf::Font font;
    font.loadFromFile("arial.ttf");
    text.setFont(font);
    text.setString(GAME_TITLE);
    text.setCharacterSize(100);
    text.setFillColor(sf::Color(0x00, 0xC0, 0xF9));
    text.setStyle(sf::Text::Bold);
    text.setPosition({pos.x + 100, pos.y + 100});

    window.draw(bg);
    window.draw(text);

    text.setString(textStr);
    text.setCharacterSize(50);
    text.setFillColor(sf::Color(0x99, 0x99, 0x99));
    text.setStyle(sf::Text::Bold);
    text.setPosition({pos.x + 100, pos.y + 400});

    window.draw(text);
}

int main()
{
    Game game;
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
        "Olimpic slalom", sf::Style::Close | sf::Style::Titlebar, settings);

    std::vector<Snow*> snowArr = {new Snow, new Snow, new Snow, new Snow, new Snow, new Snow, new Snow, new Snow, new Snow, new Snow};

    initGame(&game);
    setLevels(&game);
    initObjectPositions(&game);
    initSnow(snowArr);

    viewMode = 0;
    game.sounds.introSound.play();

    while (window.isOpen())
    {
        float deltaTime = getDeltaTime(clock);
        switch (viewMode)
        {
            case 0:
                pollEventsIntro(&viewMode, window, &game);
                updateIntro(&viewMode, &introBall, deltaTime, timer, &game);
                redrawFrameIntro(window, timer, introBall);
                window.display();
                break;
            case 1:
                pollEventsMenu(&viewMode, window, &game);
                updateSnow(snowArr, deltaTime);
                redrawFrameMenu(window, snowArr, &game, !game.isStarted ? START_WINDOW_TEXT : PAUSE_TEXT);
                window.display();
                break;
            case 2:
                pollEventsGame(&viewMode, window, &game);
                updateSnow(snowArr, deltaTime);
                updateGame(&viewMode, &game, deltaTime, &timer);
                redrawFrameGame(window, &game, snowArr, false);
                window.display();
                break;
            case 6:
                pollEventsLose(&viewMode, window, &game);
                updateSnow(snowArr, deltaTime);
                redrawFrameLoseWin(window, snowArr, &game, LOSE_TEXT_HEADER, LOSE_TEXT);
                window.display();
                break;
            case 7:
                pollEventsWin(&viewMode, window, &game, &timer);
                updateSnow(snowArr, deltaTime);
                redrawFrameLoseWin(window, snowArr, &game, WIN_TEXT_HEADER, WIN_TEXT);
                window.display();
                break;
            case 9:
                pollEventsOutro(&viewMode, window);
                redrawFrameOutro(window, timer);
                window.display();
                break;
            default:
                break;
        }
    }
}
