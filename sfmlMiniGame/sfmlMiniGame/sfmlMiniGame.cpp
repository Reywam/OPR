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

const int SPAWN_TIME = 1000;

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

	float speed = 0.5f;
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

struct GameEntities
{
	//”казатели на игровые сущности
	Hero *hero;

	list<Enemy> *enemies;

	list<Bullet> *enemyBullets;
	list<Bullet> *bullets;

	Spawner *spawner;
	Background *background;

	RenderWindow *window;
};

void InitGameEntities(GameEntities &entities, RenderWindow *window, Hero *hero, list<Enemy> *enemies
						,list <Bullet> *bullets, list<Bullet> *enemyBullets, Spawner *spawner
						, Background *background)
{
	entities.hero = hero;
	entities.window = window;
	entities.enemies = enemies;
	entities.bullets = bullets;
	entities.enemyBullets = enemyBullets;
	entities.spawner = spawner;
	entities.background = background;
}

bool LoadResourses(Resourses &res)
{
	return (
		res.textFont.loadFromFile("Resourses/albionic.ttf")
		&& res.backgroundImage.loadFromFile("Resourses/background.png")
		&& res.shipImage.loadFromFile("Resourses/hero.jpg", IntRect(13, 14, 36, 38))
		&& res.shotSound.openFromFile("Resourses/player_shot_sound.wav")
		&& res.bulletImage.loadFromFile("Resourses/laser_bullet.png", IntRect(25, 30, 50, 3))
		);
}

void BackgroundInitialization(Background &background, Resourses const &res)
{
	background.backgroundBot.setTexture(res.backgroundImage);
	background.backgroundTop.setTexture(res.backgroundImage);
	background.backgroundBot.setPosition(0, 0);
	background.backgroundTop.setPosition(0, -SCREEN_HEIGHT);
	background.backgroundBot.setScale(2, 1);
	background.backgroundTop.setScale(2, 1);
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
		bullet->bulletSprite.move(0, (float)(-bullet->speed * time));
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
				//¬зрыв, звук взрыва
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
				//¬зрыв, звук взрыва
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

template <typename T>
void DeleteObjects(list<T> &objects)
{
	objects.clear();
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

void PrintEndGameText(GameEntities *entities, Resourses const &res)
{
	entities->window->clear();

	Text endGameText;
	endGameText.setFont(res.textFont);
	endGameText.setCharacterSize(END_LEVEL_CHARACTER_SIZE);
	endGameText.setString("Your score: " + to_string(entities->spawner->scoreCount) + "\nPress R to restart");
	endGameText.setPosition(SCREEN_WIDTH/4, SCREEN_HEIGHT / 4);

	entities->window->draw(endGameText);
	
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
	if (spawner.spawnTime >= SPAWN_TIME)
	{
		spawner.spawnTime = 0;

		auto enemy = CreateEnemy(res);
		enemies.push_back(enemy);
	}
}

void GameRestart(GameEntities *entities)
{
	Vector2f defaultHeroPosition = Vector2f((float)SCREEN_WIDTH / 2.0f - (float)entities->hero->heroImage.getSize().y,
		(float)SCREEN_HEIGHT - 2.0f * (float)entities->hero->heroImage.getSize().y);

	entities->spawner->scoreCount = 0;

	entities->hero->hitPoints = HERO_HP;
	entities->hero->heroSprite.setPosition(defaultHeroPosition);

	DeleteObjects(*entities->enemies);
	DeleteObjects(*entities->bullets);
	DeleteObjects(*entities->enemyBullets);
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

void HandleEvents(GameEntities *entities, Resourses &res, float const &time)
{
	float heroSpeed = entities->hero->speed;
	Vector2f heroPosition = entities->hero->heroSprite.getPosition();

	Event event;
	while (entities->window->pollEvent(event))
	{
		if (event.type == Event::KeyPressed && event.key.code == Keyboard::Left)
		{
			entities->hero->heroSprite.setPosition(heroPosition.x - (heroSpeed * time), heroPosition.y);
		}

		if (event.type == Event::KeyPressed && event.key.code == Keyboard::Right)
		{
			entities->hero->heroSprite.setPosition(heroPosition.x + (heroSpeed * time), heroPosition.y);
		}

		if (event.type == Event::Closed || (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape))
		{
			entities->window->close();
		}

		if (event.type == Event::KeyPressed && event.key.code == Keyboard::Z && entities->hero->hitPoints > 0)
		{
			Vector2f bulletPosition = (entities->hero->heroSprite.getPosition()
				+ Vector2f((float)entities->hero->heroImage.getSize().x / 2, 0));

			Bullet bullet = CreateBullet(bulletPosition, res);
			entities->bullets->push_back(bullet);

			res.shotSound.play();
		}

		if (event.type == Event::KeyPressed &&  event.key.code == Keyboard::R)
		{
			GameRestart(entities);
		}
	}
}

void EnemyAttack(GameEntities *entities, Resourses &res)
{
	auto &collider = entities->hero->damageArea;

	for (auto enemy = entities->enemies->begin(); enemy != entities->enemies->end(); enemy++)
	{
		if (enemy->enemySprite.getGlobalBounds().intersects(collider.getGlobalBounds()) && enemy->localTime >= 1000)
		{
			enemy->localTime = 0;

			Vector2f bulletPosition = (enemy->enemySprite.getPosition()
				- Vector2f((float)res.shipImage.getSize().x / 4 + 5, (float)res.shipImage.getSize().y / 2));

			Bullet bullet = CreateBullet(bulletPosition, res);
			entities->enemyBullets->push_back(bullet);

			res.shotSound.play();
		}
	}
}

void Update(GameEntities *entities, Resourses &res, float time)
{
	entities->window->clear();

	CheckHeroMove(*entities->hero);
	SpawnEnemies(*entities->enemies, *entities->spawner, res);

	EnemyAttack(entities, res);

	MoveHeroDmgArea(*entities->hero);

	UpdateObjectsTime(*entities->spawner, time);
	UpdateObjectsTime(*entities->enemies, time);

	MoveBackground(*entities->background, time);
	CheckBackgroundMove(*entities->background);

	MoveBullets(*entities->enemyBullets, -time);
	MoveBullets(*entities->bullets, time);
	MoveEnemies(*entities->enemies, time);

	ShipsCollision(*entities->hero, *entities->enemies);

	entities->hero->hpText.setString("HP: " + to_string(entities->hero->hitPoints));
	entities->spawner->score.setString(to_string(entities->spawner->scoreCount));

	CheckCollision(*entities->bullets, *entities->enemies, entities->spawner->scoreCount);
	CheckCollision(*entities->enemyBullets, *entities->hero);
	CheckCollision(*entities->bullets, *entities->enemyBullets);

	CheckEnemyMove(*entities->enemies, *entities->spawner);
}

void Render(GameEntities *entities)
{
	entities->window->draw(entities->background->backgroundBot);
	entities->window->draw(entities->background->backgroundTop);
	entities->window->draw(entities->hero->heroSprite);

	DrawBullets(*entities->window, *entities->enemyBullets);
	DrawBullets(*entities->window, *entities->bullets);
	DrawEnemies(*entities->window, *entities->enemies);

	entities->window->draw(entities->spawner->score);
	entities->window->draw(entities->hero->hpText);
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

	GameEntities entities;
	InitGameEntities(entities, &window, &hero, &enemies, &bullets, &enemyBullets, &spawner, &background);

	while (window.isOpen())
	{
		float time = static_cast<float>(clock.getElapsedTime().asMicroseconds());
		clock.restart();
		time /= 1000;

		HandleEvents(&entities, res, time);
		bool heroIsAlive = hero.hitPoints > 0;

		if (heroIsAlive)
		{
			Update(&entities, res, time);
			Render(&entities);
		}
		else
		{
			PrintEndGameText(&entities, res);
		}
		window.display();
	}
}