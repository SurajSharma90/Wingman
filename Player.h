#pragma once

#include"Bullet.h"

class Player
{
private:
	float dtMultiplier;
	float keyTimeMax;
	float keyTime;

	unsigned playerNr;

	Vector2f playerCenter;

	float shootTimer;
	float shootTimerMax;
	float damageTimer;
	float damageTimerMax;

	Sprite sprite;
	RectangleShape hitBox;

	//Accessories
	Sprite mainGunSprite;
	dArr<Bullet> bullets;
	Texture *laserTexture;
	Texture *missile01Texture;
	Texture *missile02Texture;

	dArr<Texture>* lWingTextures;
	dArr<Texture>* rWingTextures;
	dArr<Texture>* cPitTextures;
	dArr<Texture>* auraTextures;
	Sprite lWing;
	Sprite rWing;
	Sprite cPit;
	Sprite aura;

	int lWingSelect;
	int rWingSelect;
	int cPitSelect;
	int auraSelect;

	int controls[5];
	Vector2f currentVelocity;
	float maxVelocity;
	float acceleration;
	Vector2f direction;
	float stabilizerForce;
	Vector2f normDir;

	int level;
	int exp;
	int expNext;
	int statPoints;

	int cooling; //endurance
	int plating; //vigor
	int power; //strength
	int wiring; //agility

	int hp;
	int hpMax;

	int damage;
	int damageMax;

	int score;

	int currentWeapon;

	//UPGRADES
	int mainGunLevel;
	bool dualMissiles01;
	bool dualMissiles02;

public:
	Player(std::vector<Texture> &textures,
		dArr<Texture> &lWingTextures,
		dArr<Texture> &rWingTextures, 
		dArr<Texture> &cPitTextures,
		dArr<Texture> &auraTextures,
		int UP = 22, int DOWN = 18,
		int LEFT = 0, int RIGHT = 3,
		int SHOOT = 57);
	virtual ~Player();

	//Accessors
	Bullet& getBullet(unsigned index);
	void removeBullet(unsigned index);
	inline const int getBulletsSize()const { return this->bullets.size(); }
	inline Vector2f getPosition()const { return this->sprite.getPosition(); }
	inline const String getHpAsString()const { return std::to_string(this->hp) + "/" + std::to_string(this->hpMax); }
	int getDamage()const;
	inline FloatRect getGlobalBounds()const { return this->sprite.getGlobalBounds(); }
	inline const int& getHp()const { return this->hp; }
	inline const int& getHpMax()const { return this->hpMax; }
	void takeDamage(int damage);
	inline bool isAlive()const { return this->hp > 0; }
	inline const int& getPlayerNr()const { return this->playerNr; }
	inline const int& getLevel()const { return this->level; }
	inline const int& getExp()const { return this->exp; }
	inline const int& getExpNext()const { return this->expNext; }
	inline bool gainExp(int exp)
	{ 
		this->exp += exp; 
		return this->UpdateLeveling();
	}
	inline void gainScore(int score) { this->score += score; }
	inline const int getScore()const { return this->score; }
	inline bool isDamageCooldown() { return this->damageTimer < this->damageTimerMax; }

	//Functions
	bool UpdateLeveling();
	void ChangeAccessories();
	void UpdateAccessories(const float &dt);
	void Combat(const float &dt);
	void Movement(Vector2u windowBounds, const float &dt);
	void Update(Vector2u windowBounds, const float &dt);
	void Draw(RenderTarget &target);

	//Statics
	static unsigned players;

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
};

