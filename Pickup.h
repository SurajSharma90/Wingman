#pragma once

#include"Upgrade.h"

class Pickup
{
private:
	float dtMultiplier;
	int type;

	dArr<Texture> *textures;
	Sprite sprite;

	float aliveTimerMax;
	float aliveTimer;

public:
	Pickup(
		dArr<Texture> &textures, 
		Vector2f position,
		int type,
		float duration
	);
	virtual ~Pickup();

	inline const int& getType()const { return this->type; }
	inline const bool canDelete()const { return this->aliveTimer >= this->aliveTimerMax; }
	inline Vector2f getPosition() { return this->sprite.getPosition(); }
	
	bool checkCollision(FloatRect rect);

	void Update(const float &dt);
	void Draw(RenderTarget &target);
};

