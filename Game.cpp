#include "Game.h"

enum texture { player = 0, laser01, missile01, mainGun01, enemy01 };

Game::Game(RenderWindow *window)
{
	this->window = window;
	this->window->setFramerateLimit(60);

	//Init fonts
	this->font.loadFromFile("Fonts/Dosis-Light.ttf");

	//Init textures
	this->textures.push_back(Texture());
	this->textures[player].loadFromFile("Textures/ship.png");
	this->textures.push_back(Texture());
	this->textures[laser01].loadFromFile("Textures/Guns/rayTex01.png");
	this->textures.push_back(Texture());
	this->textures[missile01].loadFromFile("Textures/Guns/missileTex01.png");
	this->textures.push_back(Texture());
	this->textures[mainGun01].loadFromFile("Textures/Guns/gun01.png");
	this->textures.push_back(Texture());
	this->textures[enemy01].loadFromFile("Textures/enemy.png");

	//Init players
	this->players.push_back(Player(this->textures));
	
	//Init enemies
	Enemy e1(
		&this->textures[enemy01], this->window->getSize(),
		Vector2f(0.f, 0.f),
		Vector2f(-1.f, 0.f), Vector2f(0.1f, 0.1f),
		0, rand() % 3 + 1, 3, 1);

	this->enemiesSaved.push_back(Enemy(e1));

	this->enemySpawnTimerMax = 10;
	this->enemySpawnTimer = this->enemySpawnTimerMax;

	this->players.push_back(Player(this->textures, 
		Keyboard::Numpad8, Keyboard::Numpad5, 
		Keyboard::Numpad4, Keyboard::Numpad6, 
		Keyboard::Numpad0));

	this->InitUI();
}

Game::~Game()
{

}

void Game::InitUI()
{
	Text tempText;

	for (size_t i = 0; i < this->players.size(); i++)
	{
		//Follow Text Init
		tempText.setFont(font);
		tempText.setCharacterSize(14);
		tempText.setColor(Color::White);
		tempText.setString(std::to_string(i));

		this->followPlayerTexts.push_back(Text(tempText));

		//Static Text Init
		tempText.setFont(font);
		tempText.setCharacterSize(14);
		tempText.setColor(Color::White);
		tempText.setString("");

		this->staticPlayerTexts.push_back(Text(tempText));
	}
}

void Game::UpdateUI()
{
	for (size_t i = 0; i < this->followPlayerTexts.size(); i++)
	{
		this->followPlayerTexts[i].setPosition(this->players[i].getPosition().x, this->players[i].getPosition().y - 20.f);
		this->followPlayerTexts[i].setString(std::to_string(i) + "					" + this->players[i].getHpAsString());
	}

	for (size_t i = 0; i < this->staticPlayerTexts.size(); i++)
	{
		
	}
}

void Game::Update()
{	
	//Update timers
	if(this->enemySpawnTimer < this->enemySpawnTimerMax)
		this->enemySpawnTimer++;

	//Spawn enemies
	if (this->enemySpawnTimer >= this->enemySpawnTimerMax)
	{
		this->enemies.push_back(Enemy(
			&this->textures[enemy01], this->window->getSize(),
			Vector2f(0.f, 0.f),
			Vector2f(-1.f, 0.f), Vector2f(0.1f, 0.1f),
			0, rand() % 3 + 1, 3, 1));

		this->enemySpawnTimer = 0; //Reset timer
	}

	for (size_t i = 0; i < this->players.size(); i++)
	{
		//UPDATE PLAYERS
		this->players[i].Update(this->window->getSize());

		//Bullets update
		for (size_t k = 0; k < this->players[i].getBullets().size(); k++)
		{
			this->players[i].getBullets()[k].Update();

			//Enemy collision check
			for (size_t j = 0; j < this->enemies.size(); j++)
			{
				if (this->players[i].getBullets()[k].getGlobalBounds().intersects(this->enemies[j].getGlobalBounds()))
				{
					this->players[i].getBullets().erase(this->players[i].getBullets().begin() + k);
					this->enemies.erase(this->enemies.begin() + j);
					return;	//RETURN!!!
				}
			}
			
			//Window bounds check
			if (this->players[i].getBullets()[k].getPosition().x > this->window->getSize().x)
			{
				this->players[i].getBullets().erase(this->players[i].getBullets().begin() + k);
				return;	//RETURN!!!
			}
		}
	}

	for (size_t i = 0; i < this->enemies.size(); i++)
	{
		this->enemies[i].Update();

		if (this->enemies[i].getPosition().x < 0 - this->enemies[i].getGlobalBounds().width)
		{
			this->enemies.erase(this->enemies.begin() + i);
			break;	//RETURN!!!
		}
	}

	//UPDATE UI
	this->UpdateUI();
}

void Game::DrawUI()
{
	for (size_t i = 0; i < this->followPlayerTexts.size(); i++)
	{
		this->window->draw(this->followPlayerTexts[i]);
	}

	for (size_t i = 0; i < this->staticPlayerTexts.size(); i++)
	{
		this->window->draw(this->staticPlayerTexts[i]);
	}
}

void Game::Draw()
{
	this->window->clear();

	for (size_t i = 0; i < this->players.size(); i++)
	{
		this->players[i].Draw(*this->window);
	}

	for (size_t i = 0; i < this->enemies.size(); i++)
	{
		this->enemies[i].Draw(*this->window);
	}

	this->DrawUI();

	this->window->display();
}