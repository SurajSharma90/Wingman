#pragma once

#include"TileArr.h"

class Stage
{
private:
	unsigned long stageSizeX;
	unsigned long stageSizeY;
	TileArr <TileArr<Tile> > tiles;
	
	//Optimization
	int fromCol, toCol;
	int fromRow, toRow;

public:
	Stage();
	virtual ~Stage();

	TileArr< TileArr<Tile> >& getTiles() { return this->tiles; }
	 
	void update(
		int fromCol, int toCol,
		int fromRow, int toRow);

	void draw(
		RenderTarget &target, 
		View &view);

	//Static variables
	static int gridSize;
};

