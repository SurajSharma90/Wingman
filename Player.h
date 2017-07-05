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
	dArr<Texture> *mainGunTextures;
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

	dArr<int> controls;
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
	int hpAdded;

	int damage;
	int damageMax;

	int score;

	int currentWeapon;

	//UPGRADES
	dArr<int> upgradesAcquired;
	int mainGunLevel;
	bool piercingShot;
	bool shield;
	bool dualMissiles01;
	bool dualMissiles02;

public:
	Player(std::vector<Texture> &textures,
		dArr<Texture> &mainGunTextures,
		dArr<Texture> &lWingTextures,
		dArr<Texture> &rWingTextures, 
		dArr<Texture> &cPitTextures,
		dArr<Texture> &auraTextures,
		int UP = Keyboard::W, 
		int DOWN = Keyboard::S,
		int LEFT = Keyboard::A, 
		int RIGHT = Keyboard::D,
		int SHOOT = Keyboard::Space,
		int STATS = Keyboard::Tab,
		int CHANGE_LWING = Keyboard::Num1,
		int CHANGE_CPIT = Keyboard::Num2,
		int CHANGE_RWING = Keyboard::Num3,
		int CHANGE_AURA = Keyboard::Num4);
	virtual ~Player();

	//Accessors
	Bullet& getBullet(unsigned index);
	void removeBullet(unsigned index);
	inline const int getBulletsSize()const { return this->bullets.size(); }
	
	inline Vector2f getPosition()const { return this->sprite.getPosition(); }
	inline void resetVelocity() { this->currentVelocity = Vector2f(0.f, 0.f); }
	inline void move(float x, float y) { this->sprite.move(x, y); this->mainGunSprite.move(x, y); }
	inline const Vector2f& getNormDir()const { return this->normDir; }
	inline FloatRect getGlobalBounds()const { return this->sprite.getGlobalBounds(); }

	inline const String getHpAsString()const { return std::to_string(this->hp) + "/" + std::to_string(this->hpMax); }
	inline const int& getHp()const { return this->hp; }
	inline const int& getHpMax()const { return this->hpMax; }
	inline bool isAlive()const { return this->hp > 0; }
	void gainHP(int hp);
	void upgradeHP();

	int getDamage()const;
	void takeDamage(int damage);
	
	inline const int& getPlayerNr()const { return this->playerNr; }
	
	inline const int& getLevel()const { return this->level; }
	inline const int& getExp()const { return this->exp; }
	inline const int& getExpNext()const { return this->expNext; }
	bool gainExp(int exp);
	inline void addStatPoint() { this->statPoints++; }
	void addStatPointRandom();
	bool playerShowStatsIsPressed();
	std::string getStatsAsString()const;
	
	inline void gainScore(int score) { this->score += score; }
	inline const int getScore()const { return this->score; }
	
	inline bool isDamageCooldown() { return this->damageTimer < this->damageTimerMax; }

	void setGunLevel(int gunLevel);
	inline const int& getGunLevel()const { return this->mainGunLevel; }
	inline void enablePiercingShot() { this->piercingShot = true; }
	inline bool getPiercingShot()const { return this->piercingShot; }
	inline void enableShield() { this->shield = true; }
	inline void enableDualMissiles01() { this->dualMissiles01 = true; }
	inline void enableDualMissiles02() { this->dualMissiles02 = true; }
	inline dArr<int>& getAcquiredUpgrades() { return this->upgradesAcquired; }

	//Functions
	void reset();
	bool updateLeveling();
	void updateStats();
	void changeAccessories(const float &dt);
	void updateAccessories(const float &dt);
	void combat(const float &dt);
	void movement(Vector2u windowBounds, const float &dt);
	void update(Vector2u windowBounds, const float &dt);
	void draw(RenderTarget &target);

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

