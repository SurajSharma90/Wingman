#include "Game.h"

enum texture { player = 0, laser01, missile01, mainGun01, enemy01 };

Game::Game(RenderWindow *window)
{
	this->window = window;
	this->window->setFramerateLimit(200);
	this->dtMultiplier = 62.5f;

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

	this->players.push_back(Player(this->textures, 
		Keyboard::Numpad8, Keyboard::Numpad5, 
		Keyboard::Numpad4, Keyboard::Numpad6, 
		Keyboard::Numpad0));
	
	this->playersAlive = this->players.size();

	//Init enemies
	Enemy e1(
		&this->textures[enemy01], this->window->getSize(),
		Vector2f(0.f, 0.f),
		Vector2f(-1.f, 0.f), Vector2f(0.1f, 0.1f),
		0, rand() % 3 + 1, 3, 1);

	this->enemiesSaved.push_back(Enemy(e1));

	this->enemySpawnTimerMax = 20;
	this->enemySpawnTimer = this->enemySpawnTimerMax;

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

	this->enemyText.setFont(this->font);
	this->enemyText.setCharacterSize(14);
	this->enemyText.setFillColor(Color::White);

	this->gameOverText.setFont(this->font);
	this->gameOverText.setFillColor(Color::Red);
	this->gameOverText.setCharacterSize(40);
	this->gameOverText.setString("GAME OVER!");
	this->gameOverText.setPosition(this->window->getSize().x/2 - 100.f, this->window->getSize().y / 2);
}

void Game::UpdateUI()
{
	for (size_t i = 0; i < this->followPlayerTexts.size(); i++)
	{
		this->followPlayerTexts[i].setPosition(this->players[i].getPosition().x, this->players[i].getPosition().y - 20.f);
		this->followPlayerTexts[i].setString(std::to_string(this->players[i].getPlayerNr()) + "					" + this->players[i].getHpAsString());
	}

	for (size_t i = 0; i < this->staticPlayerTexts.size(); i++)
	{
		
	}
}

void Game::Update(const float &dt)
{	

	if (this->playersAlive > 0)
	{
		//Update timers
		if (this->enemySpawnTimer < this->enemySpawnTimerMax)
			this->enemySpawnTimer += 1.f * dt * this->dtMultiplier;

		//Spawn enemies
		if (this->enemySpawnTimer >= this->enemySpawnTimerMax)
		{
			this->enemies.push_back(Enemy(
				&this->textures[enemy01], this->window->getSize(),
				Vector2f(0.f, 0.f),
				Vector2f(-1.f, 0.f), Vector2f(0.1f, 0.1f),
				0, rand() % 3 + 1, 2, 1));

			this->enemySpawnTimer = 0; //Reset timer
		}

		for (size_t i = 0; i < this->players.size(); i++)
		{
			if (this->players[i].isAlive())
			{
				//UPDATE PLAYERS
				this->players[i].Update(this->window->getSize(), dt);

				//Bullets update
				for (size_t k = 0; k < this->players[i].getBullets().size(); k++)
				{
					this->players[i].getBullets()[k].Update(dt);

					//Enemy collision check
					for (size_t j = 0; j < this->enemies.size(); j++)
					{
						if (this->players[i].getBullets()[k].getGlobalBounds().intersects(this->enemies[j].getGlobalBounds()))
						{
							this->players[i].getBullets().erase(this->players[i].getBullets().begin() + k);

							if (this->enemies[j].getHP() > 0)
								this->enemies[j].takeDamage(this->players[i].getDamage());

							if (this->enemies[j].getHP() <= 0)
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
		}

		for (size_t i = 0; i < this->enemies.size(); i++)
		{
			this->enemies[i].Update(dt);

			for (size_t k = 0; k < this->players.size(); k++)
			{
				if (this->players[k].isAlive())
				{
					if (this->players[k].getGlobalBounds().intersects(this->enemies[i].getGlobalBounds()))
					{
						this->players[k].takeDamage(this->enemies[i].getDamage());

						if (!this->players[k].isAlive())
							this->playersAlive--;

						this->enemies.erase(this->enemies.begin() + i);
						return;//RETURN!!!
					}
				}
			}


			if (this->enemies[i].getPosition().x < 0 - this->enemies[i].getGlobalBounds().width)
			{
				this->enemies.erase(this->enemies.begin() + i);
				return;	//RETURN!!!
			}
		}

		//UPDATE UI
		this->UpdateUI();

	}
}

void Game::DrawUI()
{
	for (size_t i = 0; i < this->followPlayerTexts.size(); i++)
	{
		if (this->players[i].isAlive())
		{
			this->window->draw(this->followPlayerTexts[i]);
		}
	}

	for (size_t i = 0; i < this->staticPlayerTexts.size(); i++)
	{
		if (this->players[i].isAlive())
		{
			this->window->draw(this->staticPlayerTexts[i]);
		}
	}

	if (this->playersAlive <= 0)
	{
		this->window->draw(this->gameOverText);
	}
}

void Game::Draw()
{
	this->window->clear();

	for (size_t i = 0; i < this->players.size(); i++)
	{
		if (this->players[i].isAlive())
		{
			this->players[i].Draw(*this->window);
		}
	}

	for (size_t i = 0; i < this->enemies.size(); i++)
	{
		this->enemyText.setPosition(this->enemies[i].getPosition());
		
		this->enemyText.setString(
			std::to_string(this->enemies[i].getHP()) + 
			"/" + 
			std::to_string(this->enemies[i].getHPMax()));

		this->enemies[i].Draw(*this->window);
		this->window->draw(this->enemyText);
	}

	this->DrawUI();

	this->window->display();
}