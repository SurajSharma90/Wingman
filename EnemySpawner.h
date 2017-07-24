#pragma once

#include"Wingman.h"

class EnemySpawner
{
private:
	Sprite sprite;
	int type; //-1 = random
	int level;
	int levelInterval;
	int nrOfEnemies; //-1 = random
	float spawnTimerMax;
	float spawnTimer;
	
public:
	EnemySpawner(
		Vector2f pos,
		int type,
		int level,
		int levelInterval,
		int nrOfEnemies,
		float spawnTimerMax
	);
	virtual ~EnemySpawner();

	void updateTimer();
	bool isInScreen(View &view);
	void spawn();

	void update(View &view);
	void draw(RenderTarget &target);
};

