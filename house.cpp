#include <SFNL/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <Color.hpp>

int main()
{

	//walls
	sf::RectangleShape walls;
	walls.setSize({ 455, 240 });
	walls.setPosition({ 55, 300 });
	walls.setFillColor(sf::Color(0x4D, 0x2E, 0x0B));

	//door
	sf::RectangleShape door;
	door.setSize({ 75, 163 });
	door.setPosition({ 100, 375 });
	door.setFillColor(sf::Color(0x18, 0x17, 0x17));

	sf::ConvexShape roof;
	roof.setFillColor(sf::Color(0x5D, 0x1E, 0x17));
	roof.setPosition(0, 0);
	roof.setPointCount(4);
	roof.setPoint(0, {1, 300});
	roof.setPoint(1, {167, 184});
	roof.setPoint(2, {400, 184});
	roof.setPoint(3, {560, 300});

	//smoke
	sf::CircleShape smoke1(25);
	smoke1.setPosition({ 400, 30 });
	smoke1.setFillColor(sf::Color(0xBF, 0xBF, 0xBF));

	sf::CircleShape smoke2(24);
	smoke2.setPosition({ 388, 60 });
	smoke2.setFillColor(sf::Color(0xBF, 0xBF, 0xBF));

	sf::CircleShape smoke3(18);
	smoke3.setPosition({ 375, 85 });
	smoke3.setFillColor(sf::Color(0xBF, 0xBF, 0xBF));

	sf::CircleShape smoke4(17);
	smoke4.setPosition({ 365, 113 });
	smoke4.setFillColor(sf::Color(0xBF, 0xBF, 0xBF));


	//truba
	sf::RectangleShape truba1;
	truba1.setSize({ 60, 40 });
	truba1.setPosition({ 340, 140 });
	truba1.setFillColor(sf::Color(0x3B, 0x38, 0x38));

	sf::RectangleShape truba2;
	truba2.setSize({ 35, 65 });
	truba2.setPosition({ 355, 180 });
	truba2.setFillColor(sf::Color(0x3B, 0x38, 0x38));

	sf::RenderWindow window(sf::VideoMode({569, 548}), 'my house');
	window.clear(White)
	window.draw(walls);
	window.draw(door);
	window.draw(roof);
	window.draw(smoke1);
	window.draw(smoke2);
	window.draw(smoke3);
	window.draw(smoke4);
	window.draw(truba1);
	window.draw(truba2);
	window.display();

	sf::sleep(sf::seconds(5));
}
