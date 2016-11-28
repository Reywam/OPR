#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <list>
#include <iterator>

const int SCREEN_WIDTH = 400;
const int SCREEN_HEIGHT = 480;

const int CHARACTER_SIZE = 40;
const int HP_TEXT_SHIFT = 120;

const int HERO_HP = 5;

using namespace sf;
using namespace std;


struct Background 
{
	Texture backgroundTexture;

	Sprite backgroundTop;
	Sprite backgroundBot;

};

struct Hero
{
	Texture heroImage;
	Sprite heroSprite;
	Music playerShotSound;

	RectangleShape damageArea;

	float speed = 0.2f;

	int hitPoints = 5;
	Text hpText;
	Font hpFont;

};

struct Enemy
{
	Texture enemyImage;
	Sprite enemySprite;

	float speed = 0.07f;
	float localTime = 0;
};

struct Bullet
{
	Texture bulletImage;
	Sprite bulletSprite;
};

struct Spawner
{
	float spawnTime = 0;
	int scoreCount = 0;
	Text score;
	Font scoreFont;
};

bool BackgroundInitialization(Background &background)
{
	if (!background.backgroundTexture.loadFromFile("Resourses/background.png"))
	{
		return false;
	}

	background.backgroundBot.setTexture(background.backgroundTexture);
	background.backgroundTop.setTexture(background.backgroundTexture);
	background.backgroundBot.setPosition(0, 0);
	background.backgroundTop.setPosition(0, -SCREEN_HEIGHT);
	background.backgroundBot.setScale(2, 1);
	background.backgroundTop.setScale(2, 1);
	return true;
}

void DrawBullets(RenderWindow &window, const list<Bullet> &bullets)
{
	for (auto bullet = bullets.begin(); bullet != bullets.end(); bullet++)
	{
		window.draw(bullet->bulletSprite);
	}
}

void DrawEnemies(RenderWindow &window, const list<Enemy> &enemies)
{
	for (auto enemy = enemies.begin(); enemy != enemies.end(); enemy++)
	{
		window.draw(enemy->enemySprite);
	}
}

void MoveEnemies(list<Enemy> &enemies, const float &time)
{
	for (auto enemy = enemies.begin(); enemy != enemies.end(); enemy++)
	{
		enemy->enemySprite.move(0, enemy->speed*time);
	}
}

void MoveBullets(list<Bullet> &bullets, const float &time)
{
	for (auto bullet = bullets.begin(); bullet != bullets.end(); bullet++)
	{
		bullet->bulletSprite.move(0, (float)(-0.5*time));
	}
}

void CheckCollision(list<Bullet> &bullets, list<Enemy> &enemies, int &score)
{
	for (auto bullet = bullets.begin(); bullet != bullets.end(); bullet++)
	{
		for (auto enemy = enemies.begin(); enemy != enemies.end(); enemy++)
		{
			bool Collision = bullet->bulletSprite.getGlobalBounds().intersects(enemy->enemySprite.getGlobalBounds());
			if (Collision)
			{
				//Взрыв, звук взрыва
				enemies.erase(enemy);
				bullets.erase(bullet);
				score += 20;
				return;
			}
		}
	}
}

void CheckCollision(list<Bullet> &bullets, list<Bullet> &enemyBullets)
{
	for (auto bullet = bullets.begin(); bullet != bullets.end(); bullet++)
	{
		for (auto enemyBullet = enemyBullets.begin(); enemyBullet != enemyBullets.end(); enemyBullet++)
		{
			bool Collision = bullet->bulletSprite.getGlobalBounds().intersects(enemyBullet->bulletSprite.getGlobalBounds());
			if (Collision)
			{
				//Взрыв, звук взрыва
				enemyBullets.erase(enemyBullet);
				bullets.erase(bullet);
				return;
			}
		}
	}
}

void CheckCollision(list<Bullet> &enemyBullets, Hero &hero)
{
	for (auto bullet = enemyBullets.begin(); bullet != enemyBullets.end(); bullet++)
	{
		bool Collision = bullet->bulletSprite.getGlobalBounds().intersects(hero.heroSprite.getGlobalBounds());
		if (Collision)
		{
			--hero.hitPoints;
			enemyBullets.erase(bullet);
			return;
		}
	}
}

void MoveBackground(Background &background, const float &time)
{
	background.backgroundBot.move(0, float(0.05 * time));
	background.backgroundTop.move(0, float(0.05 * time));
}

void CheckBackgroundMove(Background &background)
{
	Vector2f botPosition = background.backgroundBot.getPosition();
	Vector2f topPosition = background.backgroundTop.getPosition();

	if (botPosition.y >= SCREEN_HEIGHT)
	{
		background.backgroundBot.setPosition(0, -SCREEN_HEIGHT);
	}

	if (topPosition.y >= SCREEN_HEIGHT)
	{
		background.backgroundTop.setPosition(0, -SCREEN_HEIGHT);
	}
}

bool InitHero(Hero &hero)
{
	if (!hero.heroImage.loadFromFile("Resourses/hero.jpg", IntRect(13, 14, 36, 38))
		|| !hero.playerShotSound.openFromFile("Resourses/player_shot_sound.wav")
		|| !hero.hpFont.loadFromFile("Resourses/albionic.ttf"))
	{
		return false;
	}

	hero.heroSprite.setTexture(hero.heroImage);
	hero.heroSprite.setPosition(SCREEN_WIDTH / 2 - 32, SCREEN_HEIGHT - 64);

	hero.hpText.setFont(hero.hpFont);
	hero.hpText.setCharacterSize(CHARACTER_SIZE);
	hero.hpText.setPosition(SCREEN_WIDTH - HP_TEXT_SHIFT, 0);

	return true;
}

void InitBullet(Bullet &bullet, Vector2f heroPosition)
{
	bullet.bulletImage.loadFromFile("Resourses/laser_bullet.png", IntRect(25, 30, 50, 3));

	bullet.bulletSprite.setTexture(bullet.bulletImage);
	bullet.bulletSprite.setPosition(heroPosition + Vector2f(45.0f, 0));
	bullet.bulletSprite.setRotation(90);
}

void CheckHeroMove(Hero &hero)
{
	Vector2f heroPosition = hero.heroSprite.getPosition();
	if (heroPosition.x >= SCREEN_WIDTH - hero.heroImage.getSize().x)
	{
		hero.heroSprite.setPosition((float)SCREEN_WIDTH - hero.heroImage.getSize().x, heroPosition.y);
	}
	if (heroPosition.x <= 0)
	{
		hero.heroSprite.setPosition(0, heroPosition.y);
	}
}

void UpdateObjectsTime(list<Enemy> &enemies, const float &time)
{
	for (auto enemy = enemies.begin(); enemy != enemies.end(); enemy++)
	{
		enemy->localTime += time;
	}
}

void UpdateObjectsTime(Spawner &spawner, const float &time)
{
	spawner.spawnTime += time;
}

bool SetScoreFont(Spawner &spawner)
{
	if (!spawner.scoreFont.loadFromFile("Resourses/albionic.ttf"))
	{
		return false;
	}
	spawner.score.setFont(spawner.scoreFont);
	spawner.score.setCharacterSize(CHARACTER_SIZE);

	return true;
}

void InitHeroDmgArea(Hero &hero)
{ 
	hero.damageArea.setSize(Vector2f((float)hero.heroImage.getSize().y, (float)SCREEN_HEIGHT));
}

void MoveHeroDmgArea(Hero &hero)
{
	hero.damageArea.setPosition(hero.heroSprite.getPosition().x, hero.damageArea.getPosition().y);
}

void DeleteObjects(list<Enemy> &enemies)
{
	enemies.clear();
}

void DeleteObjects(list<Bullet> &bullets)
{
	bullets.clear();
}

void CheckEnemyMove(list<Enemy> &enemies, Spawner &spawner)
{
	for (auto enemy = enemies.begin(); enemy != enemies.end(); enemy++)
	{
		if (enemy->enemySprite.getPosition().y > SCREEN_HEIGHT)
		{
			spawner.scoreCount -= 40;
			enemies.erase(enemy);
			break;
		}
	}
}

int main() 
{
	RenderWindow window(VideoMode(SCREEN_WIDTH,SCREEN_HEIGHT), "Mini game");
	
	Spawner spawner;
	if (!SetScoreFont(spawner))
	{
		return 1;
	}
	
	Background background;
	if (!BackgroundInitialization(background))
	{
		return 1;
	}

	list<Bullet> bullets;
	list<Bullet> enemyBullets;

	list<Enemy> enemies;

	Hero hero;
	if (!InitHero(hero))
	{
		return 1;
	}
	InitHeroDmgArea(hero);

	Clock clock;

	Texture bulletTexture;
	bulletTexture.loadFromFile("Resourses/laser_bullet.png", IntRect(25, 30, 50, 3));

	Texture enemyTexture;
	enemyTexture.loadFromFile("Resourses/hero.jpg", IntRect(13, 14, 36, 38));

	while (window.isOpen())
	{
		float time = static_cast<float>(clock.getElapsedTime().asMicroseconds());
		clock.restart();
		time /= 1000;

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				window.close();
			}

			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
			{
				return 0;
			}

			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Left)
			{
				hero.heroSprite.move(-hero.speed * time, 0);
			}

			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Right)
			{
				hero.heroSprite.move(hero.speed * time, 0);
			}

			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Z && hero.hitPoints > 0)
			{
				// Создание снаряда
				// Текстура-недолгожитель
				Bullet bullet;

				bullet.bulletSprite.setTexture(bulletTexture);
				bullet.bulletSprite.setPosition(hero.heroSprite.getPosition() 
					+ Vector2f((float)hero.heroImage.getSize().x/2, 0));
				bullet.bulletSprite.setRotation(90);

				bullets.push_back(bullet);

				hero.playerShotSound.play();
			}

			if (event.type == Event::KeyPressed &&  event.key.code == Keyboard::R && hero.hitPoints <= 0)
			{
				spawner.scoreCount = 0;
				hero.hitPoints = HERO_HP;
				DeleteObjects(enemies);
				DeleteObjects(bullets);
				DeleteObjects(enemyBullets);
			}
		}

		window.clear();

		CheckHeroMove(hero);

		if (spawner.spawnTime >= 1000)
		{
			spawner.spawnTime = 0;
			float random = (float)(128 + rand() % (SCREEN_WIDTH - 128));
			// Текстура-недолгожитель
			Enemy enemy;
			enemy.enemyImage.loadFromFile("Resourses/hero.jpg");
			enemy.enemySprite.setTexture(enemyTexture);
			enemy.enemySprite.setPosition(random, -100);
			enemy.enemySprite.setRotation(180);
			enemies.push_back(enemy);
		}

		//Изменение данных
		UpdateObjectsTime(spawner, time);
		UpdateObjectsTime(enemies, time);

		MoveBackground(background, time);
		CheckBackgroundMove(background);

		MoveHeroDmgArea(hero);
		auto &collider = hero.damageArea;

		for (auto enemy = enemies.begin(); enemy != enemies.end(); enemy++)
		{
			if (enemy->enemySprite.getGlobalBounds().intersects(collider.getGlobalBounds()) && enemy->localTime >= 1000)
			{
				enemy->localTime = 0;
				Bullet bullet;
				// Текстура - недолгожитель
				bullet.bulletSprite.setTexture(bulletTexture);
				bullet.bulletSprite.setPosition(enemy->enemySprite.getPosition()
					- Vector2f((float)enemy->enemyImage.getSize().x/4 , (float)enemy->enemyImage.getSize().y/2));
				bullet.bulletSprite.setRotation(90);

				enemyBullets.push_back(bullet);
				hero.playerShotSound.play();
			}
		}
		
		MoveBullets(enemyBullets, -time);
		MoveBullets(bullets, time);
		MoveEnemies(enemies, time);
		
		hero.hpText.setString("HP: " + to_string(hero.hitPoints));
		spawner.score.setString(to_string(spawner.scoreCount));

		CheckCollision(bullets, enemies, spawner.scoreCount);
		CheckCollision(enemyBullets, hero);
		CheckCollision(bullets, enemyBullets);

		CheckEnemyMove(enemies, spawner);

		//Рендер
		window.draw(background.backgroundBot);
		window.draw(background.backgroundTop);
		window.draw(hero.heroSprite);

		DrawBullets(window, enemyBullets);
		DrawBullets(window, bullets);
		DrawEnemies(window, enemies);

		window.draw(spawner.score);
		window.draw(hero.hpText);

		window.display();
	}
}