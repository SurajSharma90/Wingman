#pragma once

#include"SFML\Graphics.hpp"
#include"SFML\System.hpp"
#include"SFML\Audio.hpp"
#include<iostream>
#include<vector>
#include<cstdlib>
#include<math.h>
#include<fstream>
#include<sstream>
#include"dArr.h"

using namespace sf;

class Wingman
{
public:
	Wingman() {};
	virtual ~Wingman() {};

	//Statics
	static int gridSize;
	static int backgroundSize;
	static Color backgroundColor;

	static void initTextures();
};

