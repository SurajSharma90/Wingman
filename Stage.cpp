#include "Stage.h"

//Save format
//rect rect rect rect pos pos bool bool
//texturerect, position, collider, damage

Stage::Stage(unsigned long sizeX, unsigned long sizeY)
	:stageSizeX(sizeX),
	stageSizeY(sizeY),
	tiles(stageSizeX),
	backgroundTiles(stageSizeX),
	enemySpawners(stageSizeX)
{
	this->dtMultiplier = 62.5f;
	this->fromCol = 0;
	this->toCol = 0;
	this->fromRow = 0;
	this->toRow = 0;
	
	this->scrollSpeed = 0.1f;

	for (unsigned i = 0; i < this->stageSizeX; i++) //Col
	{
		this->tiles.push(TileArr<Tile>(stageSizeY), i);
		this->backgroundTiles.push(TileArr<Tile>(stageSizeY), i);
		this->enemySpawners.push(TileArr<Tile>(stageSizeY), i);
	}
}

Stage::~Stage()
{

}

void Stage::addTile(const Tile tile, unsigned row, unsigned col, bool background)
{
	if (row >= this->stageSizeX || col >= this->stageSizeY)
		throw("OUT OF BOUNDS STAGE ADDTILE");

	if (!background)
	{
		if (this->tiles[row].isNull(col))
			this->tiles[row].push(tile, col);
		else
			std::cout << "Aleady a tile in that position!" << "\n";
	}
	else
	{
		if (this->backgroundTiles[row].isNull(col))
		{
			this->backgroundTiles[row].push(Tile(tile.getTexRect(), tile.getPos(), false, false), col);
			this->backgroundTiles[row][col].setColor(Color(100, 100, 100, 255));
		}
		else
			std::cout << "Aleady a background tile in that position!" << "\n";
	}
}

void Stage::removeTile(unsigned row, unsigned col, bool background)
{
	if (row >= this->stageSizeX || col >= this->stageSizeY)
		throw("OUT OF BOUNDS STAGE REMOVETILE");

	if (!background)
	{
		if (!this->tiles[row].isNull(col))
			this->tiles[row].remove(col);
		else
			std::cout << "No tile in that position!" << "\n";
	}
	else
	{
		if (!this->backgroundTiles[row].isNull(col))
			this->backgroundTiles[row].remove(col);
		else
			std::cout << "No background tile in that position!" << "\n";
	}
}

void Stage::saveStage(std::string fileName)
{
	std::ofstream out;

	out.open(fileName.c_str());

	if (out.is_open())
	{
		//Save map size
		out << std::to_string(this->stageSizeX) << " ";
		out << std::to_string(this->stageSizeY) << " ";

		//Save background path
		out << "NONE ";

		out << "\n"; 

		for (size_t i = 0; i < this->stageSizeX; i++)
		{
			//Regular tiles
			for (size_t k = 0; k < this->stageSizeY; k++)
			{
				if(!this->tiles[i].isNull(k))
					out << this->tiles[i][k].getAsString() << " ";
			}
		}

		out << "\n";

		for (size_t i = 0; i < this->stageSizeX; i++)
		{
			//Regular tiles
			for (size_t k = 0; k < this->stageSizeY; k++)
			{
				if (!this->backgroundTiles[i].isNull(k))
					out << this->backgroundTiles[i][k].getAsString() << " ";
			}
		}
	}
	else
		std::cout << "Could not open map file " << fileName << "\n";

	out.close();
}

bool Stage::loadStage(std::string fileName, View &view)
{
	std::ifstream in;
	bool loadSuccess = false;

	std::stringstream ss;
	std::string line = "";

	unsigned sizeX = 0;
	unsigned sizeY = 0;
	std::string backgroundPath;
	int rectLeft = 0;
	int rectTop = 0;
	int rectWidth = 0;
	int rectHeight = 0;
	int gridPosX = 0;
	int gridPosY = 0;
	bool isCollider = 0;
	bool isDamaging = 0;
	int damage = 0;

	//Open file
	in.open(fileName);

	if (in.is_open())
	{
		//Load basics
		std::getline(in, line);
		ss.str(line);

		ss >> sizeX >> sizeY >> backgroundPath;

		this->stageSizeX = sizeX;
		this->stageSizeY = sizeY;

		//Clear old stage
		this->tiles.resizeClear(this->stageSizeX);
		this->backgroundTiles.resizeClear(this->stageSizeX);
		this->enemySpawners.resizeClear(this->stageSizeX);
		
		for (size_t i = 0; i < this->stageSizeX; i++)
		{
			this->tiles.push(TileArr<Tile>(stageSizeY), i);
			this->backgroundTiles.push(TileArr<Tile>(stageSizeY), i);
			this->enemySpawners.push(TileArr<Tile>(stageSizeY), i);
		}

		line.clear();
		ss.clear();
		
		//Load regular tiles

		std::getline(in, line);
		ss.str(line);

		while (
			ss >> rectLeft >> rectTop
			>> rectWidth >> rectHeight
			>> gridPosX >> gridPosY
			>> isCollider >> isDamaging
			>> damage
			)
		{
			this->tiles[gridPosX].push(
				Tile(
					IntRect(rectLeft, rectTop, rectWidth, rectHeight),
					Vector2f(gridPosX * Wingman::gridSize, gridPosY * Wingman::gridSize),
					isCollider,
					isDamaging),
				gridPosY
			);
		}

		//Load backgroundtiles
		line.clear();
		ss.clear();

		std::getline(in, line);
		ss.str(line);

		while (
			ss >> rectLeft >> rectTop
			>> rectWidth >> rectHeight
			>> gridPosX >> gridPosY
			>> isCollider >> isDamaging
			>> damage
			)
		{
			this->backgroundTiles[gridPosX].push(
				Tile(
					IntRect(rectLeft, rectTop, rectWidth, rectHeight),
					Vector2f(gridPosX * Wingman::gridSize, gridPosY * Wingman::gridSize),
					isCollider,
					isDamaging),
				gridPosY
			);

			this->backgroundTiles[gridPosX][gridPosY].setColor(Wingman::backgroundColor);
		}

		//Enemy Spawners
		//line.clear();
		//ss.clear();

		//std::getline(in, line);
		//ss.str(line);

		//while (
	
		//	)
		//{
		//	this->enemySpawners[gridPosX].push(

		//	);
		//}

		loadSuccess = true;
	}
	else
		loadSuccess = false;

	in.close();

	return loadSuccess;
}

void Stage::updateBackground(const float &dt, unsigned row, unsigned col)
{

}

void Stage::update(const float &dt, View &view, bool editor)
{
	//this->fromCol = (view.getCenter().x - view.getSize().x / 2) / Wingman::gridSize;
	//if (fromCol <= 0)
	//	fromCol = 0;
	//if (fromCol >= this->stageSizeX)
	//	fromCol = this->stageSizeX;

	//this->toCol = (view.getCenter().x + view.getSize().x / 2) / Wingman::gridSize + 1;
	//if (toCol <= 0)
	//	toCol = 0;
	//if (toCol >= this->stageSizeX)
	//	toCol = this->stageSizeX;

	//this->fromRow = (view.getCenter().y - view.getSize().y / 2) / Wingman::gridSize;
	//if (fromRow <= 0)
	//	fromRow = 0;
	//if (fromRow >= this->stageSizeY)
	//	fromRow = this->stageSizeY;

	//this->toRow = (view.getCenter().y + view.getSize().y / 2) / Wingman::gridSize + 1;
	//if (toRow <= 0)
	//	toRow = 0;
	//if (toRow >= this->stageSizeY)
	//	toRow = this->stageSizeY;

	////std::cout << fromCol << " " << toCol << " " << fromRow << " " << toRow << "\n";

	////Tiles
	//for (int i = fromCol; i < toCol; i++)
	//{
	//	for (int k = fromRow; k < toRow; k++)
	//	{

	//		if (!this->backgroundTiles[i].isNull(k))
	//			this->backgroundTiles[i][k].update(dt);

	//		if (!this->tiles[i].isNull(k))
	//			this->tiles[i][k].update(dt);

	//		if (!this->enemySpawners[i].isNull(k))
	//			this->enemySpawners[i][k].update(dt);

	//		this->updateBackground(dt, i, k);
	//	}
	//}
}

void Stage::draw(RenderTarget &target, View &view, bool editor)
{
	//Index calculations
	this->fromCol = (view.getCenter().x - view.getSize().x / 2)/Wingman::gridSize;
	if (fromCol <= 0)
		fromCol = 0;
	if (fromCol >= this->stageSizeX)
		fromCol = this->stageSizeX;

	this->toCol = (view.getCenter().x + view.getSize().x / 2) / Wingman::gridSize + 1;
	if (toCol <= 0)
		toCol = 0;
	if (toCol >= this->stageSizeX)
		toCol = this->stageSizeX;

	this->fromRow = (view.getCenter().y - view.getSize().y / 2) / Wingman::gridSize;
	if (fromRow <= 0)
		fromRow = 0;
	if (fromRow >= this->stageSizeY)
		fromRow = this->stageSizeY;

	this->toRow = (view.getCenter().y + view.getSize().y / 2) / Wingman::gridSize + 1;
	if (toRow <= 0)
		toRow = 0;
	if (toRow >= this->stageSizeY)
		toRow = this->stageSizeY;

	//std::cout << fromCol << " " << toCol << " " << fromRow << " " << toRow << "\n";

	//Tiles
	for (int i = fromCol; i < toCol; i++)
	{
		for (int k = fromRow; k < toRow; k++)
		{
			if (!this->backgroundTiles[i].isNull(k))
				this->backgroundTiles[i][k].draw(target);

			if(!this->tiles[i].isNull(k))
				this->tiles[i][k].draw(target);

			if (!this->enemySpawners[i].isNull(k))
				this->enemySpawners[i][k].draw(target);
		}
	}
}