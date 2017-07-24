#include "EnemySpawner.h"

EnemySpawner::EnemySpawner(
	Vector2f pos,
	int type,
	int level,
	int levelInterval,
	int nrOfEnemies,
	float spawnTimerMax
)
{
	this->sprite.setPosition(pos);
	this->type = type;
	this->level = level;
	this->levelInterval = level;
	this->nrOfEnemies = nrOfEnemies;
	this->spawnTimerMax = spawnTimerMax;
	this->spawnTimer = this->spawnTimerMax;
}

EnemySpawner::~EnemySpawner()
{

}

void EnemySpawner::updateTimer()
{

}

bool EnemySpawner::isInScreen(View &view)
{

}

void EnemySpawner::spawn()
{

}

void EnemySpawner::update(View &view)
{

}

void EnemySpawner::draw(RenderTarget &target)
{
	target.draw(this->sprite);
}