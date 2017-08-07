#pragma once

#include"WButton.h"

class MainMenu
{
public:
	MainMenu();
	virtual ~MainMenu();

	void update(const float &dt);
	void draw(RenderTarget &target);

};

