#include "GameMapMaker.h"

GameMapMaker::GameMapMaker(RenderWindow *window)
{
	this->window = window;
	this->window->setFramerateLimit(300);
	this->fullscreen = false;
	this->dtMultiplier = 62.5f;

	this->stage = nullptr;

	this->keyTimeMax = 5.f;
	this->keyTime = this->keyTimeMax;

	this->initialize();
}

GameMapMaker::~GameMapMaker()
{
	delete this->stage;
}

void GameMapMaker::toggleFullscreen()
{
	if (Keyboard::isKeyPressed(Keyboard::F11) && this->keyTime >= this->keyTimeMax)
	{
		this->keyTime = 0.f;

		if (fullscreen)
		{
			this->fullscreen = false;
			this->window->close();
			this->window->create(sf::VideoMode(1920, 1080), "Wingman GameMapMaker", Style::Default);
		}
		else
		{
			this->fullscreen = true;
			this->window->close();
			this->window->create(sf::VideoMode(1920, 1080), "Wingman GameMapMaker", Style::Fullscreen);
		}
	}
}

void GameMapMaker::initView()
{
	this->mainView.setSize(Vector2f(this->window->getSize()));
	this->mainView.setCenter(Vector2f(
		this->window->getSize().x / 2,
		this->window->getSize().y / 2
	));
}

void GameMapMaker::initMapTextures()
{
	Tile::tileTextures.loadFromFile("Textures/Map/textureSheet.png");
}

void GameMapMaker::initTextures()
{
	Texture temp;

	//Map
	this->initMapTextures();
}

void GameMapMaker::initUI()
{
	this->windowUI = true;

	this->textureX = 0;
	this->textureY = 0;
	this->selector.setSize(Vector2f(Wingman::gridSize, Wingman::gridSize));
	this->selector.setFillColor(Color::Transparent);
	this->selector.setOutlineColor(Color::Red);
	this->selector.setOutlineThickness(2.f);

	this->textureSelector.setTexture(Tile::tileTextures);
}

void GameMapMaker::initMap()
{
	this->stage = new Stage(10, 10);
}

void GameMapMaker::initialize()
{
	//Init view
	this->initView();

	//Init fonts
	this->font.loadFromFile("Fonts/Dosis-Light.ttf");

	//INit textures
	this->initTextures();

	//Init map
	this->initMap();

	//Init UI
	this->initUI();
}

void GameMapMaker::updateMousePositions()
{
	this->mousePosWindow = Mouse::getPosition(*this->window);
	this->mousePosWorld = this->window->mapPixelToCoords(this->mousePosWindow);
	
	if (windowUI)
	{
		this->mousePosGrid.x = this->mousePosWindow.x / (Wingman::gridSize + 1);
		this->mousePosGrid.y = this->mousePosWindow.y / (Wingman::gridSize + 1);
	}
	else
	{
		this->mousePosGrid.x = this->mousePosWorld.x / Wingman::gridSize;
		this->mousePosGrid.y = this->mousePosWorld.y / Wingman::gridSize;
		if (this->mousePosGrid.x < 0)
			this->mousePosGrid.x = 0;
		if (this->mousePosGrid.y < 0)
			this->mousePosGrid.y = 0;
		if (this->mousePosGrid.x >= this->stage->getSizeX())
			this->mousePosGrid.x = this->stage->getSizeX() - 1;
		if (this->mousePosGrid.y >= this->stage->getSizeY())
			this->mousePosGrid.y = this->stage->getSizeY() - 1;
	}

}

void GameMapMaker::updateView(const float &dt)
{
	if (Keyboard::isKeyPressed(Keyboard::W))
	{
		this->mainView.move(0.f, -10.f * dt * this->dtMultiplier);		
	}
	else if(Keyboard::isKeyPressed(Keyboard::S))
	{
		this->mainView.move(0.f, 10.f * dt * this->dtMultiplier);
	}
	if (Keyboard::isKeyPressed(Keyboard::A))
	{
		this->mainView.move(-10.f * dt * this->dtMultiplier, 0.f);
	}
	else if (Keyboard::isKeyPressed(Keyboard::D))
	{
		this->mainView.move(10.f * dt * this->dtMultiplier, 0.f);
	}
}

void GameMapMaker::updateTimers(const float &dt)
{
	if (this->keyTime < this->keyTimeMax)
		this->keyTime += 1.f * dt * this->dtMultiplier;
}

void GameMapMaker::mapUpdate()
{

}

void GameMapMaker::updateControls()
{
	if (Keyboard::isKeyPressed(Keyboard::Tab) && this->keyTime >= this->keyTimeMax)
	{
		if(this->windowUI)
			this->windowUI = false;
		else 
			this->windowUI = true;

		std::cout << this->windowUI << "\n";

		this->keyTime = 0;
	}

	if (this->windowUI)
	{
		//Select a texture
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			this->textureX = this->mousePosGrid.x * (Wingman::gridSize + 1);
			this->textureY = this->mousePosGrid.y * (Wingman::gridSize + 1);
		}
	}
	else
	{
		this->updateAddRemoveTiles();
	}
}

void GameMapMaker::updateAddRemoveTiles()
{
	if (Mouse::isButtonPressed(Mouse::Left))
	{
		this->stage->addTile(
			Tile(
			IntRect(
				this->textureX, 
				this->textureY, 
				Wingman::gridSize, 
				Wingman::gridSize),
			Vector2f(
				this->mousePosGrid.x * Wingman::gridSize, 
				this->mousePosGrid.y * Wingman::gridSize), 
				false, 
				false),
			this->mousePosGrid.x, 
			this->mousePosGrid.y
		);
	}
	else if (Mouse::isButtonPressed(Mouse::Right))
	{
		this->stage->removeTile(this->mousePosGrid.x, this->mousePosGrid.y);
	}
}

void GameMapMaker::updateUI()
{
	if (this->windowUI)
	{
		this->selector.setPosition(
			this->mousePosGrid.x * (Wingman::gridSize + 1),
			this->mousePosGrid.y * (Wingman::gridSize + 1)
		);
	}
	else
	{
		this->selector.setPosition(
			this->mousePosGrid.x * (Wingman::gridSize),
			this->mousePosGrid.y * (Wingman::gridSize)
		);
	}
}

void GameMapMaker::update(const float &dt)
{
	//Timers UPDATE
	this->updateTimers(dt);

	//Mouse positions
	this->updateMousePositions();

	//Fullscreen
	this->toggleFullscreen();

	//Map
	this->mapUpdate();

	//General controls
	this->updateControls();

	//UI update
	this->updateUI();

	//View
	this->updateView(dt);

}

void GameMapMaker::drawUIWindow()
{
	this->window->draw(this->textureSelector);

	this->window->draw(this->selector);
}

void GameMapMaker::drawUIView()
{
	this->window->draw(this->selector);
}

void GameMapMaker::drawMap()
{
	this->stage->draw(
		*this->window,
		this->mainView
	);
}

void GameMapMaker::draw()
{
	//CLEAR AND BEGIN DRAW
	this->window->clear();

	//Set view
	this->window->setView(this->mainView);

	//Draw map
	this->drawMap();

	//Draw UI

	//Set view
	if (this->windowUI)
	{
		this->window->setView(this->window->getDefaultView());

		this->drawUIWindow();
	}
	else
	{
		this->window->setView(this->mainView);

		this->drawUIView();
	}

	//FINISH DRAW
	this->window->display();
}
