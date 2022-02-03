#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "vectors/main.h"

struct LevelData
{
    std::string name;
    int checkpoints;
    int time;
    int distance;
    bool isStones = false;
};

struct Player
{
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Vector2f position;
    float angle;
    int size = 200;
};

const int CHECKPOINT_WIDTH = 300;
struct Checkpoint
{
    sf::Vector2f position = {0, 0};
    sf::Vector2f size = {CHECKPOINT_WIDTH, 5};
    sf::RectangleShape shape;
    bool isTouched = false;
};

struct Start
{
    sf::Vector2f position;
    sf::RectangleShape shape;
    float height = 20;
};

struct Finish
{
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Vector2f position;
};

struct Background
{
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Vector2f position;
};

struct Stone
{
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Vector2f position;
};

struct Sounds
{
    sf::Music introSound;
    sf::Music gameSound;
    sf::Music outroSound;
    sf::Music winSound;
    sf::Music loseSound;
};

struct Game
{
    Background background;
    Player player;
    int points = 0;
    int time = 0;
    std::vector<Checkpoint*> checkpointsArr = {new Checkpoint, new Checkpoint};
    Start start;
    Finish finish;
    std::vector<Stone*> stone;
    int level = 0;
    std::vector<LevelData> levels;
    float rotationSpeed = 60.f;
    float movingSpeed = 200.f;
    bool isLeftPressed = false;
    bool isRightPressed = false;
    bool isStarted = false;
    Sounds sounds;
};