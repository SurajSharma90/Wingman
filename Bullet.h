#pragma once

#include"TextTag.h"

class Bullet
{
private:
	float dtMultiplier;

	Texture *texture;
	Sprite sprite;

	Vector2f currentVelocity;
	float maxVelocity;
	float acceleration;
	Vector2f direction;

public:
	Bullet(Texture *texture, 
		Vector2f position, Vector2f scale,
		Vector2f direction, float initialVelocity,
		float maxVelocity, float acceleration);
	virtual ~Bullet();

	//Accessors
	inline FloatRect getGlobalBounds()const { return this->sprite.getGlobalBounds(); }
	inline const Vector2f& getPosition()const { return this->sprite.getPosition(); }

	//Functions
	void Movement(const float &dt);

	void Update(const float &dt);
	void Draw(RenderTarget &target);
};

