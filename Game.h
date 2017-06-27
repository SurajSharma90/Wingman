#pragma once

#include"Player.h"
#include"Enemy.h"

class Game
{
private:
	RenderWindow *window;
	float dtMultiplier;

	//UI
	//Text
	Font font;
	Text followPlayerText;
	Text staticPlayerText;
	Text enemyText;
	Text gameOverText;

	RectangleShape playerExpBar;

	//Players
	std::vector<Player> players;
	int playersAlive;

	//Enemies
	std::vector<Enemy> enemies;
	std::vector<Enemy> enemiesSaved;
	float enemySpawnTimer;
	float enemySpawnTimerMax;
	
	//Textures
	std::vector<Texture> textures;
	
public:
	Game(RenderWindow *window);
	virtual ~Game();

	//Accessors
	inline RenderWindow& getWindow() { return *this->window; }

	//Setters

	//Functions
	void InitUI();
	void UpdateUIPlayer(int index);
	void UpdateUIEnemy(int index);
	void Update(const float &dt);
	void DrawUI();
	void Draw();
};

