#include "stdafx.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <cassert>

#include "Render.h"

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 500;

const int BASE_HP = 1000;

const int SPAWN_TIME = 2;
const int MINIONS_SPAWN_TIME = 20;

const int END_OF_WAVE = 50;

using namespace sf;
using namespace std;

bool InitResourses(Resourses &res)
{
	return(
		res.backgroundImage.loadFromFile("Res/tbg3.png")
		&& res.meeleEnemyImage.loadFromFile("Res/ens.png")
		&& res.rangeEnemyImage.loadFromFile("Res/TestRes/enemyArcher.png")
		&& res.baseImage.loadFromFile("Res/ens.png", IntRect(250, 0, 50, 300))
		&& res.arrowImage.loadFromFile("Res/arrow.png", IntRect(0, 1, 30, 4))
		&& res.heroImage.loadFromFile("Res/ens.png", IntRect(200, 170, 30, 40))
		&& res.hpBarImage.loadFromFile("Res/TestRes/hpbar.png")
		&& res.hpPotionImage.loadFromFile("Res/TestRes/pt1.png")
		&& res.explPotionImage.loadFromFile("Res/TestRes/bomb.png")
		&& res.explosionImage.loadFromFile("Res/TestRes/explosion.png")
		&& res.skillBarImage.loadFromFile("Res/TestRes/i_background.png", IntRect(323, 800, 315, 80))
		&& res.spellSlotImage.loadFromFile("Res/TestRes/skill.png")
		&& res.fireballImage.loadFromFile("Res/icons/fireball.png")
		&& res.healImage.loadFromFile("Res/icons/heal.png")
		&& res.enchantedWeaponImage.loadFromFile("Res/icons/enchantedWeapon.png")
		&& res.spellFireballImage.loadFromFile("Res/TestRes/fireballSpell.png")
		&& res.fireballSound.openFromFile("Res/Sounds/fireball.wav")
		&& res.fireballCastSound.openFromFile("Res/Sounds/fireballCast.wav")
		&& res.healSound.openFromFile("Res/Sounds/heal.wav")
		&& res.arrowShoot.openFromFile("Res/Sounds/shoot.ogg")
		&& res.environmentSound.openFromFile("Res/Sounds/environment.ogg")
		&& res.enemyDieSound.openFromFile("Res/Sounds/enemyDieSound.wav")
		&& res.mainThemeSound.openFromFile("Res/Sounds/Fehu.wav")
		&& res.humanEnemyDieSound.openFromFile("Res/Sounds/humanDieSound.wav")
		&& res.cursorImage.loadFromFile("Res/cursor3.png")
		&& res.magicArrowImage.loadFromFile("Res/magicArrow2.png")
		&& res.bossImage.loadFromFile("Res/boss/boss.png")
		&& res.font.loadFromFile("Res/fonts/albionic.ttf")
		&& res.loseGameImage.loadFromFile("Res/TestRes/youlose.jpg")
	);
}

void InitEndGameTexts(Resourses &res)
{
	res.youWinText.setFont(res.font);
	res.youLoseText.setFont(res.font);

	Vector2f windowCenter(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	Vector2f loseShift(100, 50);
	Vector2f winShift(70, 50);

	res.youWinText.setString("You win!");
	res.youWinText.setPosition(windowCenter - winShift);

	res.youLoseText.setString("You lose :'(.");
	res.youLoseText.setPosition(windowCenter - loseShift);
}

void SkillBarInit(SkillBar &skillBar, Resourses const &res)
{
	skillBar.sprite.setTexture(res.skillBarImage);
	skillBar.sprite.setPosition(0, 0);
	skillBar.sprite.setScale((float)1.91, 1);

	Vector2f spellIconShift(100, 0);
	Vector2f activeButtonShift(7, 3);

	skillBar.useButton1.setFont(res.font);
	skillBar.useButton1.setCharacterSize(17);
	skillBar.useButton1.setString("Q");

	skillBar.useButton2.setFont(res.font);
	skillBar.useButton2.setCharacterSize(17);
	skillBar.useButton2.setString("W");

	skillBar.useButton3.setFont(res.font);
	skillBar.useButton3.setCharacterSize(17);
	skillBar.useButton3.setString("E");

	skillBar.slot1.setTexture(res.spellSlotImage);
	skillBar.slot1.setPosition(skillBar.sprite.getPosition() + Vector2f(150, 7));
	skillBar.slot1.setScale(0.5, 0.5);
	skillBar.useButton1.setPosition(skillBar.slot1.getPosition() + activeButtonShift);

	skillBar.slot2.setTexture(res.spellSlotImage);
	skillBar.slot2.setPosition(skillBar.slot1.getPosition() + spellIconShift);
	skillBar.slot2.setScale(0.5, 0.5);
	skillBar.useButton2.setPosition(skillBar.slot2.getPosition() + activeButtonShift);

	skillBar.slot3.setTexture(res.spellSlotImage);
	skillBar.slot3.setPosition(skillBar.slot2.getPosition() + spellIconShift);
	skillBar.slot3.setScale(0.5, 0.5);
	skillBar.useButton3.setPosition(skillBar.slot3.getPosition() + activeButtonShift);

	skillBar.spell1Cooldown.setFont(res.font);
	skillBar.spell2Cooldown.setFont(res.font);
	skillBar.spell3Cooldown.setFont(res.font);

	skillBar.spell1Cooldown.setPosition(skillBar.slot1.getPosition() + Vector2f(20, 10));
	skillBar.spell2Cooldown.setPosition(skillBar.slot2.getPosition() + Vector2f(13, 10));
	skillBar.spell3Cooldown.setPosition(skillBar.slot3.getPosition() + Vector2f(13, 10));
}

void SpellsInit(SkillBar &skillBar, Resourses const &res)
{
	skillBar.spell1.setTexture(res.fireballImage);
	skillBar.spell1.setScale(0.2f, 0.2f);

	skillBar.spell2.setTexture(res.healImage);
	skillBar.spell2.setScale(0.2f, 0.2f);

	skillBar.spell3.setTexture(res.enchantedWeaponImage);
	skillBar.spell3.setScale(0.2f, 0.2f);

	Vector2f spellShift(6, 6);

	skillBar.spell1.setPosition(skillBar.slot1.getPosition() + spellShift);
	skillBar.spell2.setPosition(skillBar.slot2.getPosition() + spellShift);
	skillBar.spell3.setPosition(skillBar.slot3.getPosition() + spellShift);
}

void UpdateSkillbarCooldown(SkillBar &skillbar, Hero const &hero)
{
	skillbar.spell1Cooldown.setString(to_string((int)hero.fireballCooldown));
	skillbar.spell2Cooldown.setString(to_string((int)hero.healCooldown));
	skillbar.spell3Cooldown.setString(to_string((int)hero.enchantedArrowCooldown));
}

void HPBarInit(HPBar &hpBar, Resourses const &res)
{
	hpBar.sprite.setTexture(res.hpBarImage);
	hpBar.sprite.setPosition(50, 100);
	hpBar.sprite.setScale(1.7f, 1);

	hpBar.background.setTexture(res.hpBarImage);
	hpBar.background.setColor(Color::Green);
	hpBar.background.setScale(hpBar.sprite.getScale());
	hpBar.background.setPosition(hpBar.sprite.getPosition());
}

void BossHPBarUpdate(Boss &boss)
{
	boss.hpBar.setSize(Vector2f((float)boss.currHP / 5, 3));
	boss.hpBar.setPosition(boss.sprite.getPosition() + Vector2f(0, 3));
	boss.hpBar.setFillColor(Color::Red);
}

void EnemyHPBarUpdate(MeeleEnemy &enemy)
{
	enemy.hpBar.setSize(Vector2f((float)enemy.hp / 2, 3));
	enemy.hpBar.setPosition(enemy.sprite.getPosition() + Vector2f(0, 3));
	enemy.hpBar.setFillColor(Color::Red);
}

void EnemyHPBarUpdate(RangeEnemy &enemy)
{
	enemy.hpBar.setSize(Vector2f((float)enemy.hp, 3));
	enemy.hpBar.setPosition(enemy.sprite.getPosition() + Vector2f(23, 15));
	enemy.hpBar.setFillColor(Color::Red);
}

void BackgroundInit(Background &background, Resourses const &res)
{
	background.image = res.backgroundImage;
	background.sprite.setTexture(background.image);
}

MeeleEnemy CreateMeeleEnemy(Resourses const &res)
{
	MeeleEnemy enemy;

	enemy.sprite.setTexture(res.meeleEnemyImage);
	enemy.sprite.setTextureRect(IntRect(345, 0, 42, 50));

	enemy.hp = 100;
	enemy.damage = 20;
	enemy.attackSpeed = 2;

	return enemy;
}

void BaseInit(Base &base, Resourses const &res)
{
	base.image = res.baseImage;
	base.sprite.setTexture(base.image);
	base.sprite.setPosition((float)(600 - base.image.getSize().x), 150);

	base.hp = 1000;
}

void HeroInit(Hero &hero, Resourses const &res)
{
	hero.sprite.setTexture(res.heroImage);
	hero.sprite.setPosition(600 - 40, 230);
	hero.damage = 20;
	hero.damageSpeed = 1;

	hero.font = res.font;
}

Arrow CreateArrow(Resourses const &res, Hero const &hero, float rotation)
{
	Arrow arrow;

	arrow.sprite.setTexture(res.arrowImage);
	arrow.sprite.setPosition(hero.sprite.getPosition());
	arrow.sprite.setOrigin((float)res.arrowImage.getSize().x, (float)(res.arrowImage.getSize().y / 2));
	arrow.sprite.setRotation(rotation);

	return arrow;
}

Arrow CreateEnemyArrow(Resourses const &res, Vector2f position)
{
	Arrow arrow;

	arrow.sprite.setTexture(res.arrowImage);
	arrow.sprite.setPosition(position);
	arrow.sprite.setOrigin((float)res.arrowImage.getSize().x, (float)res.arrowImage.getSize().y / 2);

	return arrow;
}

Explosion CreateExplosion(Resourses const &res)
{
	Explosion explosion;

	explosion.sprite.setTexture(res.explosionImage);
	explosion.sprite.setTextureRect(IntRect(0, 0, 85, 85));
	explosion.sprite.setScale(1.5, 1.5);

	return explosion;
}

void ExploseAnimation(vector<Explosion> &explosions)
{
	for (auto explosion = explosions.begin(); explosion != explosions.end(); ++explosion)
	{
		if (explosion->currentFrame >= 12)
		{
			explosions.erase(explosion);
			break;
		}

		explosion->sprite.setTextureRect(IntRect(85 * (int)explosion->currentFrame, 0, 85, 85));
	}
}

Vector2f Normalize(Vector2f vector)
{
	float vectorLength = std::hypotf(vector.x, vector.y);
	if (vectorLength < std::numeric_limits<float>::epsilon())
	{
		vector = Vector2f(0, 0);
	}
	else
	{
		vector = { vector.x / vectorLength, vector.y / vectorLength };
	}

	return vector;
}

void MoveEnemy(MeeleEnemy &enemy, float time)
{
	if (enemy.currentFrame > 6)
	{
		enemy.currentFrame = 0;
	}
	enemy.sprite.setTextureRect(IntRect(345 + 40 * (int)enemy.currentFrame, 0, 42, 50));
	enemy.sprite.move(50 * time, 0);
}

void MoveEnemy(RangeEnemy &enemy, Base const &base, float time)
{
	if (enemy.currentFrame >= 3)
	{
		enemy.currentFrame = 0;
	}

	if (enemy.sprite.getPosition().x <= base.sprite.getPosition().x - 300)
	{
		enemy.sprite.setTextureRect(IntRect(55 * (int)enemy.currentFrame, 0, 55, 55));
		enemy.sprite.move(50 * time, 0);
	}
	else
	{
		enemy.state = attack;
	}
}

void Update(vector<MeeleEnemy> &enemies, float time)
{
	for (auto enemy = enemies.begin(); enemy != enemies.end(); ++enemy)
	{
		enemy->time += time;
		enemy->currentFrame += 5*time;
	}
}

void Update(vector<RangeEnemy> &enemies, float time)
{
	for (auto enemy = enemies.begin(); enemy != enemies.end(); ++enemy)
	{
		enemy->time += time;
		enemy->currentFrame += 5 * time;
	}
}

void FireballExplosion(const CircleShape &damageArea, vector<MeeleEnemy> &mEnemies, vector<RangeEnemy> &rEnemies
						, vector<BossMinion> &minions)
{
	for (auto enemy = mEnemies.begin(); enemy != mEnemies.end(); enemy++)
	{
		if (damageArea.getGlobalBounds().intersects(enemy->sprite.getGlobalBounds()))
		{
			enemy->hp -= 100;
		}
	}

	for (auto enemy = rEnemies.begin(); enemy != rEnemies.end(); enemy++)
	{
		if (damageArea.getGlobalBounds().intersects(enemy->sprite.getGlobalBounds()))
		{
			enemy->hp -= 100;
		}
	}

	for (auto enemy = minions.begin(); enemy != minions.end(); enemy++)
	{
		if (damageArea.getGlobalBounds().intersects(enemy->sprite.getGlobalBounds()))
		{
			enemy->currHP -= 100;
		}
	}
}


HealPotion CreateHealBonus(Resourses const &res, Vector2f const &position)
{
	HealPotion potion;

	potion.sprite.setTexture(res.hpPotionImage);
	potion.sprite.setPosition(position);

	potion.sprite.setScale(0.1f, 0.1f);

	return potion;
}

ExplosePotion CreateExplosionBonus(Resourses const &res, Vector2f const &position)
{
	ExplosePotion potion;

	potion.sprite.setTexture(res.explPotionImage);
	potion.sprite.setPosition(position);

	potion.sprite.setScale(0.2f, 0.2f);

	return potion;
}

void CheckCollision(vector<Arrow> &arrows, vector<MeeleEnemy> &enemies, Resourses &res, 
	vector<HealPotion> &pots, Hero const &hero)
{
	for (auto enemy = enemies.begin(); enemy != enemies.end(); ++enemy)
	{
		for (auto arrow = arrows.begin(); arrow != arrows.end(); ++arrow)
		{
			if (arrow->sprite.getGlobalBounds().intersects(enemy->sprite.getGlobalBounds()))
			{
				//res.enemyHitSound.play();
				enemy->hp -= hero.damage;
				arrows.erase(arrow);
				
				return;
			}
		}
	}
}

void CheckCollision(vector<Arrow> &arrows, vector<RangeEnemy> &enemies, Hero const &hero)
{
	for (auto enemy = enemies.begin(); enemy != enemies.end(); ++enemy)
	{
		for (auto arrow = arrows.begin(); arrow != arrows.end(); ++arrow)
		{
			if (arrow->sprite.getGlobalBounds().intersects(enemy->sprite.getGlobalBounds()))
			{
				enemy->hp -= hero.damage;
				arrows.erase(arrow);
				return;
			}
		}
	}
}

void CheckCollision(vector<HealPotion> &potions, CircleShape cursor, Base &base)
{
	for (auto pot = potions.begin(); pot != potions.end(); ++pot)
	{
		if (pot->sprite.getGlobalBounds().intersects(cursor.getGlobalBounds()))
		{
			base.hp += pot->healEffect;
			potions.erase(pot);
			return;
		}
	}
}

void CheckCollision(GameEntities &entities, vector<ExplosePotion> &potions, CircleShape cursor, vector<MeeleEnemy> &mEnemies,
	vector<RangeEnemy> &rEnemies, Resourses &res)
{
	for (auto pot = potions.begin(); pot != potions.end(); ++pot)
	{
		if (pot->sprite.getGlobalBounds().intersects(cursor.getGlobalBounds()))
		{
			res.fireballSound.play();

			CircleShape damageArea(40);
			damageArea.setPosition(pot->sprite.getPosition());

			auto explosion = CreateExplosion(res);

			explosion.sprite.setPosition(pot->sprite.getPosition() - Vector2f(45, 45));
			entities.explosions.push_back(explosion);

			FireballExplosion(damageArea, mEnemies, rEnemies, entities.boss.minions);
			potions.erase(pot);
			return;
		}
	}
}

void EnemyAttack(MeeleEnemy &enemy, Base &base)
{
	if (enemy.currentFrame > 4)
	{
		enemy.currentFrame = 0;
	}

	enemy.sprite.setTextureRect(IntRect(340 + 53 * (int)enemy.currentFrame, 45, 47, 40));
	if ((int)enemy.currentFrame >= 2 && enemy.time >= 1)
	{
		base.hp -= enemy.damage;
		enemy.time = 0;
	}
}


void EnemyAttack(Resourses const &res, RangeEnemy &enemy, Base &base, vector<Arrow> &arrows)
{
	if (enemy.currentFrame > 2)
	{
		enemy.currentFrame = 0;
		enemy.state = stay;
	}

	enemy.sprite.setTextureRect(IntRect(8 * 55 + 55 * (int)enemy.currentFrame, 0, 55, 45));

	if ((int)enemy.currentFrame >= 1 && enemy.time >= 2)
	{
		auto enemyArrow = CreateEnemyArrow(res, enemy.sprite.getPosition() + Vector2f(50, 45));
		enemyArrow.sprite.setRotation(180);

		arrows.push_back(enemyArrow);

		enemy.time = 0;
	}
}

void ProcessEnemyActivity(MeeleEnemy &enemy, Base &base, State &state, float time)
{
	switch (state)
	{
	case movement:
		MoveEnemy(enemy, time);
		break;
	case attack:
		EnemyAttack(enemy, base);
		break;
	}
}

void SetStateSprite(RangeEnemy &enemy, Resourses const &res)
{
	if (enemy.time <= 2)
	{
		enemy.sprite.setTextureRect(IntRect(0, 0, 55, 55));
	}
	else
	{
		enemy.state = attack;
	}
}

void ProcessEnemyActivity(Resourses const &res, RangeEnemy &enemy, Base &base, vector<Arrow> &arrows, State &state, float time)
{
	switch (state)
	{
	case movement:
		MoveEnemy(enemy, base, time);
		break;
	case attack:
		EnemyAttack(res, enemy, base, arrows);
		break;
	case stay:
		SetStateSprite(enemy, res);
		break;
	}
}

void ProcessEnemiesActivities(vector<MeeleEnemy> &enemies, Base &base, float time)
{
	for (auto enemy = enemies.begin(); enemy != enemies.end(); enemy++)
	{
		EnemyHPBarUpdate(*enemy);
		ProcessEnemyActivity(*enemy, base, enemy->state, time);
	}
}

void ProcessEnemiesActivities(Resourses const &res, vector<RangeEnemy> &enemies, Base &base, vector<Arrow> &arrows, float time)
{
	for (auto enemy = enemies.begin(); enemy != enemies.end(); enemy++)
	{
		EnemyHPBarUpdate(*enemy);
		ProcessEnemyActivity(res, *enemy, base, arrows, enemy->state, time);
	}
}

void CheckCollision(vector<MeeleEnemy> &enemies, Base const &base)
{
	for (auto enemy = enemies.begin(); enemy != enemies.end(); enemy++)
	{
		if (enemy->sprite.getGlobalBounds().intersects(base.sprite.getGlobalBounds()))
		{
			enemy->state = attack;
		}
	}
}

RangeEnemy CreateRangeEnemy(Resourses const &res)
{
	RangeEnemy enemy;

	enemy.sprite.setTexture(res.rangeEnemyImage);
	enemy.sprite.setTextureRect(IntRect(0, 0, 55, 55));
	enemy.sprite.setScale(1.5, 1.5);
	enemy.sprite.setPosition(100, 300);
	enemy.hp = 40;

	enemy.state = movement;

	return enemy;
}

void MoveArrows(vector<Arrow> &arrows, float time)
{
	for (auto arrow = arrows.begin(); arrow != arrows.end(); ++arrow)
	{
		Vector2f direction = Vector2f(arrow->movementVector.x * arrow->arrowSpeed,
			arrow->movementVector.y * arrow->arrowSpeed);

		arrow->sprite.move(10 * direction.x * time, 10 * direction.y * time);
	}
}

int Clamp(int value, int min, int max)
{
	assert(min < max);
	if (value < min)
	{
		value = min;
	}
	else if (value > max)
	{
		value = max;
	}
	return value;
}

void CheckBaseHP(Base &base)
{
	base.hp = Clamp(base.hp, 0, BASE_HP);
}

void MoveEnemyArrows(vector<Arrow> &arrows)
{
	for (auto &arrow : arrows)
	{
		arrow.sprite.move(10, 0);
	}
}

void CheckCollision(vector<Arrow> &arrows, Base &base)
{
	for (auto arrow = arrows.begin(); arrow != arrows.end(); arrow++)
	{
		if (arrow->sprite.getGlobalBounds().intersects(base.sprite.getGlobalBounds()))
		{
			base.hp -= 20;
			arrows.erase(arrow);
			return;
		}
	}
}

Fireball CreateFireball(Resourses const &res, const Vector2f &position)
{
	Fireball fireball;
	fireball.sprite.setTexture(res.spellFireballImage);
	fireball.sprite.setTextureRect(IntRect(330, 0, 290, 140));

	fireball.sprite.setScale(0.5, 0.5);

	fireball.sprite.setPosition(position);

	fireball.damage = 100;
	return fireball;
}

void MoveFireball(Fireball &fireball, float time)
{
	if (fireball.currentFrame > 6)
	{
		fireball.currentFrame = 0;
	}

	if ((int)fireball.currentFrame < 3)
	{
		fireball.sprite.setTextureRect(IntRect(330 * (int)fireball.currentFrame, 0, 290, 140));
	}
	else
	{
		fireball.sprite.setTextureRect(IntRect(330 * (int)(fireball.currentFrame - 3), 140, 290, 140));
	}

	Vector2f movementVector = Vector2f(fireball.direction.x * fireball.movementSpeed,
		fireball.direction.y * fireball.movementSpeed);

	fireball.sprite.move(30 * movementVector.x * time, 30 * movementVector.y * time);
}

void MoveFireballs(vector<Fireball> &fireballs, float time)
{
	for (auto &fireball : fireballs)
	{
		MoveFireball(fireball, time);
	}
}

void Update(vector<Fireball> &fireballs, float time)
{
	for (auto &fireball : fireballs)
	{
		fireball.Update(time);
	}
}

void SpellCooldown(Hero &hero, float time)
{
	if (hero.fireballCooldown > 0)
	{
		hero.fireballCooldown -= time;
	}

	if (hero.healCooldown > 0)
	{
		hero.healCooldown -= time;
	}

	if (hero.enchantedArrowCooldown)
	{
		hero.healCooldown -= time;
	}
}

Vector2f CalculateDirection(const Vector2f &pos1, const Vector2f &pos2)
{
	Vector2f direction = Vector2f(pos2.x - pos1.x, pos2.y - pos1.y);

	direction = Normalize(direction);

	return direction;
}

CircleShape CheckFireballsPosition(vector<Fireball> &fireballs)
{
	for (auto fireball = fireballs.begin(); fireball != fireballs.end(); fireball++)
	{
		if (fireball->sprite.getPosition().x <= fireball->explosionPosition.x)
		{
			CircleShape explosionArea(50);
			explosionArea.setFillColor(Color::Red);
			Vector2f explosionShift(explosionArea.getRadius(), explosionArea.getRadius());

			explosionArea.setPosition(fireball->sprite.getPosition() - explosionShift);

			fireballs.erase(fireball);

			return explosionArea;
		}
	}

	return CircleShape(0);
}

void CheckEnemiesHp(vector<MeeleEnemy> &enemies, Potions &pots, Resourses &res, Spawner &spawner)
{
	for (auto enemy = enemies.begin(); enemy != enemies.end(); enemy++)
	{
		if (enemy->hp <= 0)
		{
			if ((int)(rand() % 100 >= 70))
			{
				pots.hPots.push_back(CreateHealBonus(res, enemy->sprite.getPosition()));
			}

			if ((int)(rand() % 100 >= 80))
			{
				pots.exPots.push_back(CreateExplosionBonus(res, enemy->sprite.getPosition()));
			}


			res.enemyDieSound.play();
			++spawner.deadEnemiesCount;
			
			enemies.erase(enemy);
			return;
		}
	}
}

void CheckEnemiesHp(vector<RangeEnemy> &enemies, Potions &pots, Resourses &res, Spawner &spawner)
{
	for (auto enemy = enemies.begin(); enemy != enemies.end(); enemy++)
	{
		if (enemy->hp <= 0)
		{
			if ((int)(rand() % 100 >= 70))
			{
				pots.hPots.push_back(CreateHealBonus(res, enemy->sprite.getPosition()));
			}

			res.humanEnemyDieSound.play();
			++spawner.deadEnemiesCount;

			enemies.erase(enemy);
			return;
		}
	}
}

void CursorInit(Cursor &cursor, Resourses const &res)
{
	cursor.cursorCollider.setRadius(10);
	cursor.sprite.setTexture(res.cursorImage);
}

void InitGameEntities(GameEntities &entities, Resourses const &res)
{
	BackgroundInit(entities.background, res);
	BaseInit(entities.base, res);
	CursorInit(entities.cursor, res);
	HeroInit(entities.hero, res);
	HPBarInit(entities.hpbar, res);
	SkillBarInit(entities.skillbar, res);
	SpellsInit(entities.skillbar, res);
}

void HandleEvents(RenderWindow &window, GameEntities &entities, Resourses &res)
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			window.close();
		}

		if (event.type == Event::KeyPressed && event.key.code == Keyboard::Q && entities.hero.fireballCooldown <= 0)
		{
			entities.hero.readyToFireballCast = true;
			entities.hero.fireballCooldown = 5;
		}

		if (event.type == Event::KeyPressed && event.key.code == Keyboard::W && entities.hero.healCooldown <= 0)
		{
			res.healSound.play();

			entities.base.hp += 500;

			entities.hero.healCooldown = 60;
		}

		if (event.type == Event::KeyPressed && event.key.code == Keyboard::E && entities.hero.enchantedArrowCooldown <= 0)
		{
			entities.hero.damage = entities.hero.damage * 2;
			entities.spawner.enchantArrowEffect = 7;
			entities.hero.enchantedArrowCooldown = 30;
		}
	}
}

void UpdateCursorPosition(RenderWindow &window, Cursor &cursor, Resourses const &res)
{
	cursor.position = static_cast<Vector2f>(Mouse::getPosition(window));

	cursor.sprite.setPosition(cursor.position - Vector2f((float)res.cursorImage.getSize().x / 2,
		(float)res.cursorImage.getSize().y / 2));
	cursor.cursorCollider.setPosition(cursor.position);
}

float CalculateRotation(Vector2f rotationVector)
{
	return (float)((atan2(rotationVector.y, rotationVector.x)) * 180 / M_PI + 180);
}

void HeroAttack(GameEntities &entities, Resourses &res, const Vector2f normalRotationVector, float rotation)
{
	if (entities.hero.damageTime >= entities.hero.damageSpeed)
	{
		// ¬ектор движени€
		entities.heroArrow.movementVector = normalRotationVector;
		entities.heroArrows.push_back(entities.heroArrow);

		// —оздали стрелу, дальше она вращаетс€ за курсором и все счастливы
		entities.heroArrow = CreateArrow(res, entities.hero, rotation);

		res.arrowShoot.play();
		entities.hero.damageTime = 0;
	}
}

void CastFireball(GameEntities &entities, Resourses &res, float rotation)
{
	res.fireballCastSound.play();

	auto fireball = CreateFireball(res, entities.hero.sprite.getPosition());

	fireball.direction = CalculateDirection(entities.hero.sprite.getPosition(), entities.cursor.position);

	fireball.explosionPosition = entities.cursor.position;
	fireball.sprite.setRotation(rotation);
	entities.fireballs.push_back(fireball);

	entities.hero.readyToFireballCast = false;
}

void SpawnEnemies(GameEntities &entities, Resourses const &res)
{
	if (entities.spawner.time >= SPAWN_TIME)
	{
		if (rand() % 2 == 0)
		{
			auto enemy = CreateMeeleEnemy(res);

			float randomYPosition = (float)(entities.base.sprite.getPosition().y + 50 + rand() % (SCREEN_HEIGHT - 300));

			enemy.sprite.setPosition(100, randomYPosition);
			entities.enemies.mEnemies.push_back(enemy);
		}
		else
		{
			auto enemy = CreateRangeEnemy(res);

			float randomYPosition = (float)(entities.base.sprite.getPosition().y + 50 + rand() % (SCREEN_HEIGHT - 300));

			enemy.sprite.setPosition(100, randomYPosition);
			entities.enemies.rEnemies.push_back(enemy);
		}

		entities.spawner.time = 0;
	}
}

void CalculateHowMuchHPWeWillDraw(GameEntities &entities, Resourses const &res)
{
	const float hpFraction = float(entities.base.hp) / float(BASE_HP);
	const int hpBarWidth = int(hpFraction * res.hpBarImage.getSize().x);
	entities.hpbar.sprite.setTextureRect(IntRect(0, 0, hpBarWidth,
		res.hpBarImage.getSize().y));
}

void Update(vector<Explosion> &explosions, float time)
{
	for (auto &explosion : explosions)
	{
		explosion.currentFrame += 10 * time;
	}
}

void EnvironmentSoundInit(Resourses &res)
{
	res.environmentSound.setLoop(true);
	res.environmentSound.play();
}

void MainThemeSoundInit(Resourses &res)
{
	res.mainThemeSound.setLoop(true);
	res.mainThemeSound.setVolume(20);
	res.mainThemeSound.play();
}

// Boss activities

void CreateBoss(GameEntities &entities, Resourses const &res)
{
	entities.boss.sprite.setTexture(res.bossImage);
	entities.boss.sprite.setTextureRect(IntRect(15, 190, 90, 110));

	entities.boss.sprite.setPosition(-100, (int)SCREEN_HEIGHT / 2);
}

void ChangeBossState(Boss &boss)
{
	if (boss.state != dead)
	{
		if (boss.sprite.getPosition().x <= 40)
		{
			boss.sprite.move(3, 0);
		}

		if (boss.sprite.getPosition().y <= ((int)SCREEN_HEIGHT / 2 - 30))
		{
			boss.state = down;
		}
		else if (boss.sprite.getPosition().y >= ((int)SCREEN_HEIGHT / 2 + 30))
		{
			boss.state = up;
		}

		if (boss.currHP <= 0)
		{
			boss.state = dead;
		}
	}
}

void BossActivities(Boss &boss)
{
	switch (boss.state)
	{
	case down:
		boss.sprite.move(0, 1);
		break;
	case up:
		boss.sprite.move(0, -1);
		break;
	case dead:
		cout << "Kek" << endl;
		boss.sprite.move(-3, 0);
		break;
	default:
		break;
	}
}

void Update(Boss &boss, float time)
{
	boss.minionSpawnTimer += time;
	boss.currentFrame += time;
}

BossMinion CreateBossMinion(Boss const &boss, Resourses const &res)
{
	BossMinion minion;

	minion.sprite.setTexture(res.bossImage);
	minion.sprite.setTextureRect(IntRect(135, 15, 30, 35));

	int randomHeightShift = rand() % 100;
	int randomWidthShift = rand() % 100;
	minion.sprite.setPosition(boss.sprite.getPosition() + Vector2f((float)randomWidthShift, (float)randomHeightShift));

	return minion;
}

void CheckCollision(Hero const &hero, vector<Arrow> &arrows, vector<BossMinion> &minions)
{
	for (auto minion = minions.begin(); minion != minions.end(); ++minion)
	{
		for (auto arrow = arrows.begin(); arrow != arrows.end(); ++arrow)
		{
			if (arrow->sprite.getGlobalBounds().intersects(minion->sprite.getGlobalBounds()))
			{
				minion->currHP -= hero.damage;
				arrows.erase(arrow);
				return;
			}
		}
	}
}

void CheckMinionsSpawnTime(Boss &boss, Resourses const &res)
{
	if (boss.minionSpawnTimer>= MINIONS_SPAWN_TIME)
	{
		int numberOfMinions = rand() % 4 + 2;

		for (size_t i = 0; i < (size_t)numberOfMinions; ++i)
		{
			auto minion = CreateBossMinion(boss, res);
			boss.minions.push_back(minion);
		}

		boss.minionSpawnTimer = 0;
	}
}

void MoveMinions(vector<BossMinion> &minions)
{
	for (auto &minion : minions)
	{
		minion.sprite.move(2, 0);
	}
}

void CheckCollision(Base &base, vector<BossMinion> &minions)
{
	for (auto minion = minions.begin(); minion != minions.end(); ++minion)
	{
		if (minion->sprite.getGlobalBounds().intersects(base.sprite.getGlobalBounds()))
		{
			base.hp -= 100;
			minion->currHP = 0;
		}
	}
}

void CheckMinionsHP(vector<BossMinion> &minions)
{
	for (auto minion = minions.begin(); minion != minions.end(); ++minion)
	{
		if (minion->currHP <= 0)
		{
			minions.erase(minion);
			break;
		}
	}
}


void Update(RenderWindow &window, GameEntities &entities, Resourses &res, float time)
{
	entities.hero.damageTime += time;
	entities.spawner.time += time;

	UpdateCursorPosition(window, entities.cursor, res);
	UpdateSkillbarCooldown(entities.skillbar, entities.hero);

	const Vector2f rotationVector = entities.cursor.position - entities.heroArrow.sprite.getPosition();
	const Vector2f normalRotationVector = Normalize(rotationVector);
	const float rotation = CalculateRotation(normalRotationVector);

	entities.heroArrow.sprite.setRotation(rotation);
	//heroArrow.sprite.setTexture(res.magicArrowImage);

	MoveArrows(entities.heroArrows, time);
	MoveEnemyArrows(entities.enemyArrows);

	if (entities.hero.readyToFireballCast)
	{
		CastFireball(entities, res, rotation);
		entities.hero.readyToFireballCast = false;
	}

	MoveFireballs(entities.fireballs, time);
	CircleShape damageArea = CheckFireballsPosition(entities.fireballs);

	bool weAreReadyForBOOM = damageArea.getRadius() != 0;
	if (weAreReadyForBOOM)
	{
		FireballExplosion(damageArea, entities.enemies.mEnemies, entities.enemies.rEnemies, entities.boss.minions);

		auto explosion = CreateExplosion(res);
		explosion.sprite.setPosition(damageArea.getPosition());
		entities.explosions.push_back(explosion);

		res.fireballSound.play();
	}

	HeroAttack(entities, res, normalRotationVector, rotation);
	if (entities.spawner.enchantArrowEffect <= 0)
	{
		entities.hero.damage /= 2;
		entities.spawner.enchantArrowEffect = 7;
	}

	Update(entities.enemies.mEnemies, time);
	Update(entities.enemies.rEnemies, time);
	Update(entities.fireballs, time);
	entities.hero.enchantedArrowCooldown -= time;
	if (entities.hero.enchantedArrowCooldown >= 22)
	{
		entities.spawner.enchantArrowEffect -= time;
	}
	Update(entities.explosions, time);
	ExploseAnimation(entities.explosions);

	SpellCooldown(entities.hero, time);

	ProcessEnemiesActivities(entities.enemies.mEnemies, entities.base, time);
	ProcessEnemiesActivities(res, entities.enemies.rEnemies, entities.base, entities.enemyArrows, time);

	SpawnEnemies(entities, res);

	CheckBaseHP(entities.base);
	CalculateHowMuchHPWeWillDraw(entities, res);

	CheckCollision(entities.enemies.mEnemies, entities.base);

	CheckCollision(entities.pots.hPots, entities.cursor.cursorCollider, entities.base);
	CheckCollision(entities, entities.pots.exPots, entities.cursor.cursorCollider, entities.enemies.mEnemies, entities.enemies.rEnemies, res);

	CheckCollision(entities.heroArrows, entities.enemies.mEnemies, res, entities.pots.hPots, entities.hero);
	CheckCollision(entities.enemyArrows, entities.base);
	CheckCollision(entities.heroArrows, entities.enemies.rEnemies, entities.hero);

	CheckEnemiesHp(entities.enemies.mEnemies, entities.pots, res, entities.spawner);
	CheckEnemiesHp(entities.enemies.rEnemies, entities.pots, res, entities.spawner);

	if (entities.spawner.deadEnemiesCount >= END_OF_WAVE && entities.boss.currentFrame == 0)
	{
		CreateBoss(entities, res);
		entities.boss.currentFrame++;
	}
}

void CheckCollision(Hero const &hero, vector<Arrow> &arrows, Boss &boss)
{
	for (auto arrow = arrows.begin(); arrow != arrows.end(); ++arrow)
	{
		if (arrow->sprite.getGlobalBounds().intersects(boss.sprite.getGlobalBounds()))
		{
			boss.currHP -= hero.damage;
			arrows.erase(arrow);
			break;
		}
	}
}

void BossUpdate(GameEntities &entities, Resourses const &res, float time)
{
	ChangeBossState(entities.boss);
	BossActivities(entities.boss);

	BossHPBarUpdate(entities.boss);
	Update(entities.boss, time);
	CheckMinionsSpawnTime(entities.boss, res);
	MoveMinions(entities.boss.minions);

	CheckCollision(entities.hero, entities.heroArrows, entities.boss.minions);
	CheckCollision(entities.base, entities.boss.minions);
	CheckCollision(entities.hero, entities.heroArrows, entities.boss);

	CheckMinionsHP(entities.boss.minions);
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Game");

	window.setMouseCursorVisible(false);

	Resourses res;

	if (!InitResourses(res))
	{
		return 1;
	}

	InitEndGameTexts(res);

	GameEntities entities;
	InitGameEntities(entities, res);

	EnvironmentSoundInit(res);
	MainThemeSoundInit(res);

	Clock clock;

	entities.heroArrow = CreateArrow(res, entities.hero, 0);


	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();

		HandleEvents(window, entities, res);

		if (entities.boss.currHP != 0 && entities.base.hp != 0)
		{
			Update(window, entities, res, time);

			if (entities.spawner.deadEnemiesCount >= END_OF_WAVE)
			{
				BossUpdate(entities, res, time);
			}
			Render(window, entities);
		}

		if (entities.boss.currHP <= 0)
		{
			cout << "WIN!" << endl;
			window.clear();

			window.draw(res.youWinText);

			window.display();
		}

		if (entities.base.hp <= 0)
		{
			cout << "LOSE!" << endl;
			
			window.clear();

			window.draw(res.youLoseText);

			window.display();
		}
	}

	

	return 0;
}