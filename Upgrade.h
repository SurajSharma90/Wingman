#pragma once

#include"SFML\Graphics.hpp"
#include"SFML\System.hpp"
#include"SFML\Audio.hpp"
#include<iostream>
#include<vector>
#include<cstdlib>
#include<math.h>
#include <fstream>
#include"dArr.h"

using namespace sf;

class Upgrade
{
private:
	float dtMultiplier;

	int type;
	
	float aliveTimerMax;
	float aliveTimer;

	dArr<Texture> *textures;
	Sprite sprite;

public:
	Upgrade(dArr<Texture> &textures,	
		Vector2f position,
		int type,
		float aliveTimerMax);

	virtual ~Upgrade();
	
	inline const int& getType()const { return this->type; }
	inline bool canDelete() { return this->aliveTimer >= this->aliveTimerMax; }
	
	bool checkCollision(FloatRect rect);

	void Update(const float &dt);
	void Draw(RenderTarget &target);
};

