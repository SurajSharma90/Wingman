#pragma once

#include"Player.h"
#include"Enemy.h"
#include"Boss.h"
#include"Map.h"

class GameMapMaker
{
private:

	//GameMapMaker
	RenderWindow *window;
	View mainView;
	float dtMultiplier;
	float keyTimeMax;
	float keyTime;
	bool fullscreen;

	//Mouse positions
	Vector2i mousePosWindow;
	Vector2f mousePosWorld;
	Vector2i mousePosGrid;

	//UI
	bool windowUI;

	int textureX;
	int textureY;
	RectangleShape selector;

	Sprite textureSelector;

	//Text
	Font font;

	//MAP
	Stage *stage;

public:
	GameMapMaker(RenderWindow *window);
	virtual ~GameMapMaker();

	//Accessors
	inline RenderWindow& getWindow() { return *this->window; }

	//Setters

	//Functions
	void toggleFullscreen();

	void initView();
	void initMapTextures();
	void initTextures();
	void initUI();
	void initMap();
	void initialize();

	void updateMousePositions();
	void updateView(const float &dt);
	void updateTimers(const float &dt);
	void mapUpdate();
	void updateControls();
	void updateAddRemoveTiles();
	void updateUI();
	void update(const float &dt);

	void drawUIWindow();
	void drawUIView();

	void drawMap();
	void draw();
};

