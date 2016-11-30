#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <list>
#include <iterator>

const int SCREEN_WIDTH = 400;
const int SCREEN_HEIGHT = 480;

const int CHARACTER_SIZE = 40;
const int END_LEVEL_CHARACTER_SIZE = 20;
const int HP_TEXT_SHIFT = 120;

const int HERO_HP = 5;

using namespace sf;
using namespace std;


struct Background 
{
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

	int hitPoints = HERO_HP;
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
};

struct Resourses
{
	Font textFont;

	Texture backgroundImage;
	Texture shipImage;
	Texture bulletImage;

	Music shotSound;
};

bool LoadResourses(Resourses &res)
{
	if (
		!res.textFont.loadFromFile("Resourses/albionic.ttf")
		|| !res.backgroundImage.loadFromFile("Resourses/background.png")
		|| !res.shipImage.loadFromFile("Resourses/hero.jpg", IntRect(13, 14, 36, 38))
		|| !res.shotSound.openFromFile("Resourses/player_shot_sound.wav")
		|| !res.bulletImage.loadFromFile("Resourses/laser_bullet.png", IntRect(25, 30, 50, 3))
		)
	{
		return false;
	}

	return true;
}

bool BackgroundInitialization(Background &background, Resourses const &res)
{
	background.backgroundBot.setTexture(res.backgroundImage);
	background.backgroundTop.setTexture(res.backgroundImage);
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

bool InitHero(Hero &hero, Resourses const &res)
{
	hero.heroImage = res.shipImage;
	hero.heroSprite.setTexture(hero.heroImage);

	hero.heroSprite.setPosition((float)SCREEN_WIDTH / 2.0f - (float)hero.heroImage.getSize().y,
		(float)SCREEN_HEIGHT - 2.0f * (float)hero.heroImage.getSize().y);

	hero.hpText.setFont(res.textFont);
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

Bullet CreateBullet(const Vector2f &position, Resourses const &res)
{
	Bullet bullet;

	bullet.bulletImage = res.bulletImage;
	bullet.bulletSprite.setTexture(res.bulletImage);
	bullet.bulletSprite.setPosition(position);
	bullet.bulletSprite.setRotation(90);

	return bullet;
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

void PrintEndGameText(RenderWindow &window, const int &score)
{
	Font font;
	if (!font.loadFromFile("Resourses/albionic.ttf"))
	{
		return;
	}

	Text endGameText;
	endGameText.setFont(font);
	endGameText.setCharacterSize(END_LEVEL_CHARACTER_SIZE);
	endGameText.setString("Your score: " + to_string(score) + "\nPress R to restart");
	endGameText.setPosition(SCREEN_WIDTH/4, SCREEN_HEIGHT / 4);

	window.draw(endGameText);
	
}

Enemy CreateEnemy(Resourses const &res)
{
	float randomXPorition = (float)(128 + rand() % (SCREEN_WIDTH - 128));

	Enemy enemy;

	enemy.enemySprite.setTexture(res.shipImage);
	enemy.enemySprite.setPosition(randomXPorition, -100);
	enemy.enemySprite.setRotation(180);

	return enemy;
}

void SpawnEnemies(list<Enemy> &enemies, Spawner &spawner, Resourses const &res)
{
	if (spawner.spawnTime >= 1000)
	{
		spawner.spawnTime = 0;

		auto enemy = CreateEnemy(res);
		enemies.push_back(enemy);
	}
}

void GameRestart(list<Enemy> &enemies, list<Bullet> &bullets, list<Bullet> &enemyBullets,
				Hero &hero, Spawner &spawner)
{
	spawner.scoreCount = 0;
	hero.hitPoints = HERO_HP;
	hero.heroSprite.setPosition((float)SCREEN_WIDTH / 2.0f - (float)hero.heroImage.getSize().y,
		(float)SCREEN_HEIGHT - 2.0f * (float)hero.heroImage.getSize().y);
	DeleteObjects(enemies);
	DeleteObjects(bullets);
	DeleteObjects(enemyBullets);
}

void ShipsCollision(Hero &hero, list<Enemy> enemies)
{
	for (auto enemy = enemies.begin(); enemy != enemies.end(); enemy++)
	{
		bool collision = hero.heroSprite.getGlobalBounds().intersects(enemy->enemySprite.getGlobalBounds());
		if (collision)
		{
			enemies.erase(enemy);
			hero.hitPoints = 0;

			break;
		}
	}
}

int main() 
{
	RenderWindow window(VideoMode(SCREEN_WIDTH,SCREEN_HEIGHT), "Mini game");
	
	Resourses res;
	if (!LoadResourses(res))
	{
		return 1;
	}

	Spawner spawner;
	spawner.score.setFont(res.textFont);
	
	Background background;
	BackgroundInitialization(background, res);

	list<Bullet> bullets;
	list<Bullet> enemyBullets;

	list<Enemy> enemies;

	Hero hero;
	InitHero(hero, res);
	InitHeroDmgArea(hero);

	Clock clock;

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

			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Right)
			{
				hero.heroSprite.move(hero.speed * time, 0);
			}

			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Z && hero.hitPoints > 0)
			{
				Vector2f bulletPosition = (hero.heroSprite.getPosition()
					+ Vector2f((float)res.shipImage.getSize().x/2, 0));

				Bullet bullet = CreateBullet(bulletPosition, res);
				bullets.push_back(bullet);

				res.shotSound.play();
			}

			if (event.type == Event::KeyPressed &&  event.key.code == Keyboard::R)
			{
				GameRestart(enemies, bullets, enemyBullets, hero, spawner);
			}
		}

		window.clear();

		if (hero.hitPoints > 0)
		{
			CheckHeroMove(hero);
			SpawnEnemies(enemies, spawner, res);

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

					Vector2f bulletPosition = (enemy->enemySprite.getPosition()
						- Vector2f((float)res.shipImage.getSize().x / 4, (float)res.shipImage.getSize().y / 2));

					Bullet bullet = CreateBullet(bulletPosition, res);
					enemyBullets.push_back(bullet);

					res.shotSound.play();
				}
			}


			MoveBullets(enemyBullets, -time);
			MoveBullets(bullets, time);
			MoveEnemies(enemies, time);

			ShipsCollision(hero, enemies);

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
		}
		else
		{
			PrintEndGameText(window, spawner.scoreCount);
		}
		window.display();
	}
}