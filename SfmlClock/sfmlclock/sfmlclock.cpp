#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <ctime>
#include <math.h>
#include <iostream>

const float CLOCK_CIRCLE_THICKNESS = 2;
const float CLOCK_CIRCLE_SIZE = 250;

const int DOTS_COUNT = 60;
const int NUMBERS_COUNT = 12;
const int POINTS_COUNT = 100;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const float HOUR_HAND_WIDTH = 5;
const float HOUR_HAND_HEIGHT = 180;

const float MINUTES_HAND_WIDTH = 3;
const float MINUTES_HAND_HEIGHT = 240;

const float SECONDS_HAND_WIDTH = 2;
const float SECONDS_HAND_HEIGHT = 250;

const float CENTER_CIRCLE_RADIUS = 10;
const float SMALL_POINT_RADIUS = 2;
const float BIG_POINT_RADIUS = 5;

const int NUMBER_SIZE = 40;

const unsigned int ANTI_ALIASING_LEVEL = 8;

using namespace sf;
using namespace std;

struct ClockFace
{
	CircleShape clockCircle;
	CircleShape centerCircle;
	Texture backgroundImage;
	Texture clockBrandSprite;
	Sprite brandSprite;
	Text numbers[NUMBERS_COUNT];
	Font numbersFont;
	CircleShape dots[DOTS_COUNT];
};

struct Hands
{
	RectangleShape secondsHand;
	RectangleShape minuteHand;
	RectangleShape hourHand;
};

struct AnalogClock
{
	ClockFace clockFace;
	Hands clockHands;
	Music clockSound;
	ContextSettings settings;
};

void NumberInit(ClockFace &clockFace, size_t i, Vector2f position)
{
	clockFace.numbers[i].setFont(clockFace.numbersFont);
	clockFace.numbers[i].setString(to_string(i + 1));
	clockFace.numbers[i].setCharacterSize(NUMBER_SIZE);
	clockFace.numbers[i].setFillColor(sf::Color::Black);

	clockFace.numbers[i].setOrigin(clockFace.numbers[i].getGlobalBounds().width / 2, clockFace.numbers[i].getGlobalBounds().height / 2 + 5);
	clockFace.numbers[i].setPosition(position.x + SCREEN_WIDTH / 2, position.y + SCREEN_HEIGHT / 2);
}


void ToCartesianCoordinates(Vector2f &position, float shift, float angle)
{
	position.x = (CLOCK_CIRCLE_SIZE - shift) * cos(angle);
	position.y = (CLOCK_CIRCLE_SIZE - shift) * sin(angle);
}

void DotsAndNumbersInitialization(ClockFace &clockFace, CircleShape dot[], Text numbers[])
{
	float angle =  static_cast<float>(M_PI + (M_PI/1.5f));

	for (int i = 0; i < DOTS_COUNT; i++)
	{
		Vector2f dotsPosition;
		Vector2f numbersPosition;

		ToCartesianCoordinates(dotsPosition, 10, angle);
		ToCartesianCoordinates(numbersPosition, 40, angle);

		Vector2f windowCenter(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

		if (i % 5 == 0)
		{
			dot[i] = sf::CircleShape(BIG_POINT_RADIUS);
			//Инициализация номера
			NumberInit(clockFace, i/5, numbersPosition);
		}
		else
		{
			dot[i] = sf::CircleShape(SMALL_POINT_RADIUS);
		}
		dot[i].setFillColor(sf::Color::Black);
		dot[i].setOrigin(dot[i].getGlobalBounds().width / 2, dot[i].getGlobalBounds().height / 2);
		dot[i].setPosition(dotsPosition + windowCenter);

		angle = static_cast<float>(angle + ((2 * M_PI) / DOTS_COUNT));
	}
}

void DrawDots(RenderWindow &window, CircleShape dots[])
{
	for (int i = 0; i < DOTS_COUNT; i++)
	{
		window.draw(dots[i]);
	}
}

void DrawNumbers(RenderWindow &window, Text numbers[])
{
	for (int i = 0; i < NUMBERS_COUNT; i++)
	{
		window.draw(numbers[i]);
	}
}

void HandsInitialization(Hands &clockHands, Vector2f &windowCenter)
{
	clockHands.hourHand.setSize(Vector2f(HOUR_HAND_WIDTH, HOUR_HAND_HEIGHT));
	clockHands.minuteHand.setSize(Vector2f(MINUTES_HAND_WIDTH, MINUTES_HAND_HEIGHT));
	clockHands.secondsHand.setSize(Vector2f(SECONDS_HAND_WIDTH, SECONDS_HAND_HEIGHT));

	clockHands.hourHand.setFillColor(sf::Color::Black);
	clockHands.minuteHand.setFillColor(sf::Color::Black);
	clockHands.secondsHand.setFillColor(sf::Color::Red);

	clockHands.hourHand.setOrigin(clockHands.hourHand.getGlobalBounds().width / 2, clockHands.hourHand.getGlobalBounds().height - 25);
	clockHands.minuteHand.setOrigin(clockHands.minuteHand.getGlobalBounds().width / 2, clockHands.minuteHand.getGlobalBounds().height - 25);
	clockHands.secondsHand.setOrigin(clockHands.secondsHand.getGlobalBounds().width / 2, clockHands.secondsHand.getGlobalBounds().height - 25);

	clockHands.hourHand.setPosition(windowCenter);
	clockHands.minuteHand.setPosition(windowCenter);
	clockHands.secondsHand.setPosition(windowCenter);
}

void SetHandsRotation(Hands &clockHands)
{
	std::time_t currentTime = std::time(NULL);

	struct tm * ptm = localtime(&currentTime);

	clockHands.hourHand.setRotation(static_cast<float>(ptm->tm_hour * 30 + (ptm->tm_min / 2)));
	clockHands.minuteHand.setRotation(static_cast<float>(ptm->tm_min * 6 + (ptm->tm_sec / 12)));
	clockHands.secondsHand.setRotation(static_cast<float>(ptm->tm_sec * 6));
}

void DrawHands(RenderWindow &window, AnalogClock &clock)
{
	Hands clockHands;
	
	sf::Vector2f windowCenter = sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
	HandsInitialization(clockHands, windowCenter);
	SetHandsRotation(clockHands);

	window.draw(clockHands.secondsHand);
	window.draw(clockHands.minuteHand);
	window.draw(clockHands.hourHand);
}

void SetBackgroundImage(ClockFace &clockFace, Texture &clockImage)
{
	clockFace.clockCircle.setTexture(&clockImage);
	clockFace.clockCircle.setTextureRect(sf::IntRect(0, 0, 200, 175));
}

void SetClockBrandSprite(ClockFace &clockFace, Texture &brandImage, Sprite &clockBrandSprite)
{
	clockBrandSprite.setTexture(brandImage);
	clockBrandSprite.setOrigin(clockBrandSprite.getTextureRect().left + clockBrandSprite.getTextureRect().width / 2.0f,
		clockBrandSprite.getTextureRect().top + clockBrandSprite.getTextureRect().height/ 1.0f);

	clockBrandSprite.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 100); 
}

bool ClockFaceInitialization(ClockFace &clockFace)
{
	if (!clockFace.backgroundImage.loadFromFile("clock-image.jpg") 
		|| !clockFace.clockBrandSprite.loadFromFile("sfml-logo.png") 
		|| !clockFace.numbersFont.loadFromFile("albionic.ttf"))
	{
		return false;
	}

	clockFace.clockCircle.setRadius(CLOCK_CIRCLE_SIZE);
	clockFace.clockCircle.setPointCount(POINTS_COUNT);
	clockFace.clockCircle.setOutlineThickness(CLOCK_CIRCLE_THICKNESS);
	clockFace.clockCircle.setOutlineColor(sf::Color::Black);
	clockFace.clockCircle.setOrigin(clockFace.clockCircle.getGlobalBounds().width / 2, clockFace.clockCircle.getGlobalBounds().height / 2);
	clockFace.clockCircle.setPosition(SCREEN_WIDTH / 2 + CLOCK_CIRCLE_THICKNESS, SCREEN_HEIGHT / 2 + CLOCK_CIRCLE_THICKNESS);

	DotsAndNumbersInitialization(clockFace, clockFace.dots, clockFace.numbers);
	SetBackgroundImage(clockFace, clockFace.backgroundImage);
	SetClockBrandSprite(clockFace, clockFace.clockBrandSprite, clockFace.brandSprite);
	return true;
}

void CenterCircleInitialization(ClockFace &clockFace, Vector2f &windowCenter)
{
	clockFace.centerCircle.setRadius(CENTER_CIRCLE_RADIUS);
	clockFace.centerCircle.setPointCount(POINTS_COUNT);
	clockFace.centerCircle.setFillColor(sf::Color::Red);
	clockFace.centerCircle.setOrigin(clockFace.centerCircle.getGlobalBounds().width / 2, clockFace.centerCircle.getGlobalBounds().height / 2);
	clockFace.centerCircle.setPosition(windowCenter);
}

bool DrawClock(RenderWindow &window, AnalogClock &clock)
{
	sf::Vector2f windowCenter = sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

	ClockFace clockFace;

	if (!ClockFaceInitialization(clockFace))
	{
		return false;
	}

	CenterCircleInitialization(clockFace, windowCenter);

	window.draw(clockFace.clockCircle);
	window.draw(clockFace.brandSprite);
	DrawHands(window, clock);
	window.draw(clockFace.centerCircle);
	DrawDots(window, clockFace.dots);
	DrawNumbers(window, clockFace.numbers);

	return true;
}

bool SetMusic(Music &clockTick)
{
	if (!clockTick.openFromFile("clock-1.wav"))
	{
		return false;
	}
	clockTick.setLoop(true);
	clockTick.play();
	return true;
}

void PlayTick(AnalogClock &clock)
{
	SetMusic(clock.clockSound);
}

void UserEvents(RenderWindow &window)
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window.close();
	}
}

int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = ANTI_ALIASING_LEVEL;

	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "SFML Analog Clock", sf::Style::Close, settings);

	AnalogClock myClock;
	PlayTick(myClock);

	while (window.isOpen())
	{
		UserEvents(window);

		window.clear(sf::Color::White);

		if (!DrawClock(window, myClock))
		{
			return 1;
		}

		window.display();
	}

	return 0;
}