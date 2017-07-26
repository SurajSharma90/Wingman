#pragma once

#include"Bullet.h"

class Enemy
{
private:
	float dtMultiplier;

	Sprite sprite;
	Vector2f moveDirection;
	Vector2f normalizedMoveDir;
	Vector2f lookDirection;
	Vector2f normalizedLookDir;
	float maxVelocity;

	float damageTimerMax;
	float damageTimer;

	float shootTimerMax;
	float shootTimer;

	int type;
	int hp;
	int hpMax;
	int damageMin;
	int damageMax;

	int playerFollowNr;

public:
	Enemy(
		View& view,
		Vector2f position, 
		Vector2f direction, 
		int type, 
		int scalar,
		int playerFollowNr);

	virtual ~Enemy();

	//Accessors
	inline const int getDamage()const { return rand() % this->damageMax + this->damageMin; }
	inline const int getHP()const { return this->hp; }
	inline const int getHPMax()const { return this->hpMax; }
	inline const bool isDead()const { return this->hp <= 0; }
	inline FloatRect getGlobalBounds()const { return this->sprite.getGlobalBounds(); }
	inline Vector2f getPosition()const { return this->sprite.getPosition(); }
	inline const int& getPlayerFollowNr()const { return this->playerFollowNr; }

	//Functions
	void collision();
	void takeDamage(int damage);
	void Update(const float &dt, Vector2f playerPosition);
	void Draw(RenderTarget &target);

	//Regular functions
	float vectorLength(Vector2f v)
	{
		return sqrt(pow(v.x, 2) + pow(v.y, 2));
	}

	Vector2f normalize(Vector2f v, float length)
	{
		if (length == 0)
			return Vector2f(0.f, 0.f);
		else
			return v / length;
	}

	//Static
	static dArr<Bullet> enemyBullets;

	static dArr<Texture> textures;
	static int nrOfTextures;

	static void initTextures();

	enum eTypes { 
		MOVELEFT = 0, 
		FOLLOW, 
		MOVELEFTSHOOT, 
		FOLLOWFAST, 
		FOLLOWSHOOT, 
		FOLLOWFASTSHOOT };
	
	enum weapons { 
		REGULAR = 0, 
		MISSILE };
};

