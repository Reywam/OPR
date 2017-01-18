#pragma once
#include "stdafx.h"

struct Resourses
{
	sf::Texture cursorImage;

	sf::Texture backgroundImage;
	sf::Texture baseImage;

	sf::Texture meeleEnemyImage;
	sf::Texture rangeEnemyImage;
	sf::Texture cannonImage;

	sf::Texture heroImage;
	sf::Texture arrowImage;
	sf::Texture magicArrowImage;

	sf::Texture hpBarImage;

	sf::Texture hpPotionImage;
	sf::Texture explPotionImage;
	sf::Texture explosionImage;

	sf::Texture skillBarImage;
	sf::Texture spellSlotImage;

	sf::Texture fireballImage;
	sf::Texture healImage;
	sf::Texture enchantedWeaponImage;

	// Boss and his minions
	sf::Texture bossImage;
	sf::Texture minionImage;

	sf::Texture loseGameImage;

	sf::Texture spellFireballImage;

	sf::Music arrowShoot;

	sf::Music fireballSound;
	sf::Music fireballCastSound;

	sf::Music healSound;
	sf::Music enchantSound;

	sf::Music enemyHitSound;
	sf::Music enemyDieSound;

	sf::Music humanEnemyDieSound;

	sf::Music environmentSound;

	sf::Music mainThemeSound;

	sf::Font font;

	sf::Text youLoseText;
	sf::Text youWinText;
};

enum State
{
	movement,
	attack,
	stay,
	up,
	down, 
	dead,
};

struct GameMenu
{
	sf::Text newGameText;
	sf::Text exitText;
};

struct HPBar
{
	sf::Sprite sprite;
	sf::Sprite background;
};

struct Hero
{
	sf::Texture image;
	sf::Sprite sprite;
	sf::Font font;

	float time = 0;

	int damage = 0;
	float damageSpeed = 0;
	float damageTime = 0;

	float fireballCooldown = 0;
	float healCooldown = 0;
	float enchantedArrowCooldown = 0;

	bool readyToFireballCast = false;
};

struct Arrow
{
	sf::Texture image;
	sf::Sprite sprite;

	sf::Vector2f movementVector;

	float dx = 0;
	float dy = 0;

	float time = 0;

	float arrowSpeed = 10;
};

struct MeeleEnemy
{
	sf::Texture image;
	sf::Sprite sprite;
	sf::RectangleShape hpBar;

	float time = 0;
	float currentFrame = 0;
	float timeToChooseMovement = 0;

	int direction = 0;

	int hp = 0;
	int damage = 0;
	int attackSpeed = 0;

	State state = movement;
};

struct RangeEnemy
{
	sf::Texture image;
	sf::Sprite sprite;
	sf::RectangleShape hpBar;

	float time = 0;
	float currentFrame = 0;

	int hp = 0;
	int damage = 0;
	int attackSpeed = 0;

	State state = movement;
};

struct Base
{
	sf::Texture image;
	sf::Sprite sprite;

	int hp = 0;
};

struct Background
{
	sf::Texture image;
	sf::Sprite sprite;
};

struct Spawner
{
	float time = 0;
	float enchantArrowEffect = 5;

	int deadEnemiesCount = 0;
};

struct HealPotion
{
	sf::Sprite sprite;

	int healEffect = 50;
};

struct ExplosePotion
{
	sf::Sprite sprite;

	int damageEffect = 50;
};

struct Bonuses
{
	HealPotion healPotion;
};

struct SkillBar
{
	sf::Sprite sprite;

	sf::Sprite slot1;
	sf::Sprite slot2;
	sf::Sprite slot3;

	sf::Sprite spell1;
	sf::Sprite spell2;
	sf::Sprite spell3;

	sf::Vector2f defaultSpell1Position;
	sf::Vector2f defaultSpell2Position;
	sf::Vector2f defaultSpell3Position;

	sf::Text useButton1;
	sf::Text useButton2;
	sf::Text useButton3;

	sf::Text spell1Cooldown;
	sf::Text spell2Cooldown;
	sf::Text spell3Cooldown;
};

struct Fireball
{
	sf::Sprite sprite;

	float damage;

	float currentFrame = 0;
	float movementSpeed = 20;

	sf::Vector2f direction;
	sf::Vector2f explosionPosition;

	void Update(float time)
	{
		currentFrame += 10 * time;
	}
};

struct Enemies
{
	std::vector<MeeleEnemy> mEnemies;
	std::vector<RangeEnemy> rEnemies;
};

struct Potions
{
	std::vector<HealPotion> hPots;
	std::vector<ExplosePotion> exPots;
};

struct Cursor
{
	sf::Sprite sprite;
	sf::CircleShape cursorCollider;

	sf::Vector2f position;
};

struct Explosion
{
	sf::Sprite sprite;
	float currentFrame = 0;
};

struct BossMinion
{
	sf::Sprite sprite;

	int currHP = 80;

	State state = movement;
};

struct Boss
{
	sf::Sprite sprite;
	sf::RectangleShape hpBar;

	int currHP = 500;

	float minionSpawnTimer = 0;
	float currentFrame = 0;

	State state = up;

	std::vector<BossMinion> minions;
};

struct GameEntities
{
	GameMenu menu;

	Hero hero;
	Arrow heroArrow;
	Base base;

	Boss boss;

	std::vector<Arrow> heroArrows;
	std::vector<Arrow> enemyArrows;

	std::vector<Fireball> fireballs;
	std::vector<Explosion> explosions;

	Enemies enemies;
	Potions pots;

	Background background;

	Spawner spawner;

	SkillBar skillbar;
	HPBar hpbar;
	Cursor cursor;
};