#include "Stage.h"

//Save format
//rect rect rect rect pos pos bool bool
//texturerect, position, collider, damage

int Stage::gridSize = 50;

Stage::Stage()
	:stageSizeX(100), 
	stageSizeY(100), 
	tiles(stageSizeX)
{
	this->fromCol = 0;
	this->toCol = 0;
	this->fromRow = 0;
	this->toRow = 0;

	for (unsigned i = 0; i < this->stageSizeX; i++) //Col
	{
		this->tiles.push(TileArr<Tile>(stageSizeY), i);
	}
}

Stage::~Stage()
{

}

void Stage::update(
	int fromCol, int toCol,
	int fromRow, int toRow)
{

}

void Stage::draw(
	RenderTarget &target, 
	View &view)
{
	this->fromCol = (view.getCenter().x - view.getSize().x / 2)/Stage::gridSize;
	if (fromCol <= 0)
		fromCol = 0;
	if (fromCol >= this->stageSizeX)
		fromCol = this->stageSizeX;

	this->toCol = (view.getCenter().x + view.getSize().x / 2) / Stage::gridSize + 1;
	if (toCol <= 0)
		toCol = 0;
	if (toCol >= this->stageSizeX)
		toCol = this->stageSizeX;

	this->fromRow = (view.getCenter().y - view.getSize().y / 2) / Stage::gridSize;
	if (fromRow <= 0)
		fromRow = 0;
	if (fromRow >= this->stageSizeY)
		fromRow = this->stageSizeY;

	this->toRow = (view.getCenter().y + view.getSize().y / 2) / Stage::gridSize + 1;
	if (toRow <= 0)
		toRow = 0;
	if (toRow >= this->stageSizeY)
		toRow = this->stageSizeY;

	//std::cout << fromCol << " " << toCol << " " << fromRow << " " << toRow << "\n";

	for (int i = fromCol; i < toCol; i++)
	{
		for (int k = fromRow; k < toRow; k++)
		{
			if(!this->tiles[i].isNull(k))
				this->tiles[i][k].draw(target);
		}
	}
}