#pragma once

#include"Player.h"
#include"Enemy.h"

class Game
{
private:

	//Game
	RenderWindow *window;
	float dtMultiplier;
	bool paused;
	float keyTimeMax;
	float keyTime;
	bool fullscreen;

	//Score
	unsigned score;
	unsigned scoreMultiplier;
	Clock scoreTimer;
	int scoreTime;
	float multiplierTimerMax;
	float multiplierTimer;
	int multiplierAdder;
	int multiplierAdderMax;
	double bestScoreSecond;
	float difficultyTimer;
	int difficulty;

	//UI
	//Text
	Font font;
	Text followPlayerText;
	Text staticPlayerText;
	Text enemyText;
	Text gameOverText;
	Text scoreText;
	Text controlsText;

	//Bars
	RectangleShape playerExpBar;

	//Texttags
	dArr<TextTag> textTags;

	//Players
	dArr<Player> players;
	int playersAlive;

	//Enemies
	dArr<Enemy> enemies;
	std::vector<Enemy> enemiesSaved;
	float enemySpawnTimer;
	float enemySpawnTimerMax;
	
	//Pickups
	dArr<Pickup> pickups;

	//Upgrades
	dArr<Upgrade> upgrades;

	//Textures
	std::vector<Texture> textures;
	dArr<Texture> playerMainGunTextures;
	dArr<Texture> enemyTextures;
	dArr<Texture> enemyBulletTextures;
	dArr<Texture> lWingTextures;
	dArr<Texture> rWingTextures;
	dArr<Texture> cPitTextures;
	dArr<Texture> auraTextures;
	dArr<Texture> pickupTextures;
	dArr<Texture> upgradeTextures;

public:
	Game(RenderWindow *window);
	virtual ~Game();

	//Accessors
	inline RenderWindow& getWindow() { return *this->window; }

	//Setters

	//Functions
	void ToggleFullscreen();
	void InitTextures();
	void InitUI();
	void UpdateUIPlayer(int index);
	void UpdateUIEnemy(int index);
	void Update(const float &dt);
	void DrawUI();
	void Draw();
};

