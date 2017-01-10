#pragma once
#include "stdafx.h"

template <typename T>
void DrawObjects(sf::RenderWindow &window, std::vector<T> const &objects)
{
	for (auto object = objects.begin(); object != objects.end(); ++object)
	{
		window.draw(object->sprite);
	}
}

void DrawEnemies(sf::RenderWindow &window, std::vector<MeeleEnemy> const &objects)
{
	for (auto object = objects.begin(); object != objects.end(); ++object)
	{
		window.draw(object->sprite);
		window.draw(object->hpBar);
	}
}

void DrawEnemies(sf::RenderWindow &window, std::vector<RangeEnemy> const &objects)
{
	for (auto object = objects.begin(); object != objects.end(); ++object)
	{
		window.draw(object->sprite);
		window.draw(object->hpBar);
	}
}

void DrawHPBar(sf::RenderWindow &window, HPBar const &hpBar)
{
	window.draw(hpBar.background);
	window.draw(hpBar.sprite);
}

void DrawSkillbar(sf::RenderWindow &window, SkillBar const &skillBar, Hero const &hero)
{
	window.draw(skillBar.sprite);

	window.draw(skillBar.slot1);
	window.draw(skillBar.slot2);
	window.draw(skillBar.slot3);

	window.draw(skillBar.spell1);
	window.draw(skillBar.spell2);
	window.draw(skillBar.spell3);

	window.draw(skillBar.useButton1);
	window.draw(skillBar.useButton2);
	window.draw(skillBar.useButton3);

	if (hero.fireballCooldown > 0)
	{
		window.draw(skillBar.spell1Cooldown);
	}
	if (hero.healCooldown > 0)
	{
		window.draw(skillBar.spell2Cooldown);
	}
	if (hero.enchantedArrowCooldown > 0)
	{
		window.draw(skillBar.spell3Cooldown);
	}
}

void Render(sf::RenderWindow &window, GameEntities &entities)
{
	window.clear();

	window.draw(entities.background.sprite);

	window.draw(entities.base.sprite);

	window.draw(entities.hero.sprite);

	DrawEnemies(window, entities.enemies.mEnemies);
	DrawEnemies(window, entities.enemies.rEnemies);

	DrawObjects(window, entities.heroArrows);
	DrawObjects(window, entities.enemyArrows);

	DrawObjects(window, entities.pots.hPots);
	DrawObjects(window, entities.pots.exPots);

	DrawObjects(window, entities.fireballs);
	DrawObjects(window, entities.explosions);

	window.draw(entities.heroArrow.sprite);
	
	window.draw(entities.boss.sprite);
	window.draw(entities.boss.hpBar);
	DrawObjects(window, entities.boss.minions);

	DrawSkillbar(window, entities.skillbar, entities.hero);
	DrawHPBar(window, entities.hpbar);
	window.draw(entities.cursor.sprite);

	sf::Text deadEnemiesCount;
	deadEnemiesCount.setFont(entities.hero.font);
	if (entities.spawner.deadEnemiesCount > 50)
	{
		entities.spawner.deadEnemiesCount = 50;
	}
	deadEnemiesCount.setString(std::to_string(entities.spawner.deadEnemiesCount) + "/50");
	deadEnemiesCount.setPosition(30, 20);

	window.draw(deadEnemiesCount);

	window.display();
}