#include "Game.h"

enum texture { player = 0, laser01, missile01, mainGun01 };

Game::Game(RenderWindow *window)
{
	this->window = window;
	//this->window->setFramerateLimit(200);
	this->dtMultiplier = 62.5f;

	//Init fonts
	this->font.loadFromFile("Fonts/Dosis-Light.ttf");

	//INit textures
	this->InitTextures();

	//Init players
	this->players.add(Player(this->textures, 
		this->lWingTextures, this->rWingTextures, 
		this->cPitTextures, this->auraTextures));

	//this->players.add(Player(this->textures,
	//	this->lWingTextures, this->rWingTextures,
	//	this->cPitTextures, this->auraTextures,
	//	Keyboard::Numpad8, Keyboard::Numpad5, 
	//	Keyboard::Numpad4, Keyboard::Numpad6, 
	//	Keyboard::Numpad1));
	
	this->playersAlive = this->players.size();

	//Init enemies

	this->enemySpawnTimerMax = 25.f;
	this->enemySpawnTimer = this->enemySpawnTimerMax;

	this->InitUI();
}

Game::~Game()
{

}

void Game::InitTextures()
{
	//Init textures regular
	this->textures.push_back(Texture());
	this->textures[player].loadFromFile("Textures/ship.png");
	this->textures.push_back(Texture());
	this->textures[laser01].loadFromFile("Textures/Guns/rayTex01.png");
	this->textures.push_back(Texture());
	this->textures[missile01].loadFromFile("Textures/Guns/missileTex01.png");
	this->textures.push_back(Texture());
	this->textures[mainGun01].loadFromFile("Textures/Guns/gun01.png");
	
	Texture temp;
	temp.loadFromFile("Textures/enemyMoveLeft.png");
	this->enemyTextures.add(Texture(temp));
	temp.loadFromFile("Textures/enemyFollow.png");
	this->enemyTextures.add(Texture(temp));

	//INit accessory textures
	std::ifstream in;

	in.open("Textures/Accessories/leftwings.txt");
	std::string fileName = "";

	if (in.is_open())
	{
		while (getline(in, fileName))
		{
			Texture temp;
			temp.loadFromFile(fileName);
			this->lWingTextures.add(Texture(temp));
		}
	}

	in.close();

	in.open("Textures/Accessories/rightwings.txt");
	fileName = "";

	if (in.is_open())
	{
		while (getline(in, fileName))
		{
			Texture temp;
			temp.loadFromFile(fileName);
			this->rWingTextures.add(Texture(temp));
		}
	}

	in.close();

	in.open("Textures/Accessories/cockpits.txt");
	fileName = "";

	if (in.is_open())
	{
		while (getline(in, fileName))
		{
			Texture temp;
			temp.loadFromFile(fileName);
			this->cPitTextures.add(Texture(temp));
		}
	}

	in.close();

	in.open("Textures/Accessories/auras.txt");
	fileName = "";

	if (in.is_open())
	{
		while (getline(in, fileName))
		{
			Texture temp;
			temp.loadFromFile(fileName);
			this->auraTextures.add(Texture(temp));
		}
	}

	in.close();
}

void Game::InitUI()
{
	Text tempText;

	//Follow Text Init
	this->followPlayerText.setFont(font);
	this->followPlayerText.setCharacterSize(14);
	this->followPlayerText.setColor(Color::White);

	//Static Text Init
	this->staticPlayerText.setFont(font);
	this->staticPlayerText.setCharacterSize(14);
	this->staticPlayerText.setColor(Color::White);
	
	this->playerExpBar.setSize(Vector2f(90.f, 10.f));
	this->playerExpBar.setFillColor(Color(0.f, 90.f, 200.f, 200.f));

	this->enemyText.setFont(this->font);
	this->enemyText.setCharacterSize(14);
	this->enemyText.setFillColor(Color::White);

	this->gameOverText.setFont(this->font);
	this->gameOverText.setFillColor(Color::Red);
	this->gameOverText.setCharacterSize(40);
	this->gameOverText.setString("GAME OVER!");
	this->gameOverText.setPosition(this->window->getSize().x/2 - 100.f, this->window->getSize().y / 2);
}

void Game::UpdateUIPlayer(int index)
{
	if (index >= this->players.size())
		std::cout << "OUT OF BOUNDS! (UPDATEUI)";
	else //FOLLOW TEXT
	{
		this->followPlayerText.setPosition(
			this->players[index].getPosition().x - 25.f,
			this->players[index].getPosition().y - 68.f
		);

		this->followPlayerText.setString(
			std::to_string(this->players[index].getPlayerNr())
			+ "								"
			+ this->players[index].getHpAsString()
			+ "\n\n\n\n\n\n\n\n\n\n"
			+ std::to_string(this->players[index].getLevel())
		);

		//BARS
		this->playerExpBar.setPosition(
			this->players[index].getPosition().x + 10.f,
			this->players[index].getPosition().y + 115.f
		);

		this->playerExpBar.setScale(
			(static_cast<float>(this->players[index].getExp())/this->players[index].getExpNext()),
			1.f
		);

		//STATIC TEXT
	}
}

void Game::UpdateUIEnemy(int index)
{
	this->enemyText.setPosition(
		this->enemies[index].getPosition().x,
		this->enemies[index].getPosition().y - 15.f
	);

	this->enemyText.setString(
		std::to_string(this->enemies[index].getHP()) +
		"/" +
		std::to_string(this->enemies[index].getHPMax()));
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
			this->enemies.add(Enemy(
				this->enemyTextures, this->window->getSize(),
				Vector2f(0.f, 0.f),
				Vector2f(-1.f, 0.f), Vector2f(0.1f, 0.1f),
				rand()%2, rand() % 3 + 1, 2, 1, rand()%this->playersAlive)
			);

			this->enemySpawnTimer = 0; //Reset timer
		}

		//Update players, bullets and combat
		for (size_t i = 0; i < this->players.size(); i++)
		{
			if (this->players[i].isAlive())
			{
				//UPDATE PLAYERS
				this->players[i].Update(this->window->getSize(), dt);

				//Bullets update
				for (size_t k = 0; k < this->players[i].getBulletsSize(); k++)
				{
					this->players[i].getBullet(k).Update(dt);

					//Enemy collision check
					for (size_t j = 0; j < this->enemies.size(); j++)
					{
						if (this->players[i].getBullet(k).getGlobalBounds().intersects(this->enemies[j].getGlobalBounds()))
						{
							this->players[i].removeBullet(k);

							//Enemy take damage
							int damage = this->players[i].getDamage();
							if (this->enemies[j].getHP() > 0)
							{
								this->enemies[j].takeDamage(damage);

								//Create text tag
								this->textTags.add(
									TextTag(&this->font,
										"-" + std::to_string(damage),
										Color::Red,
										Vector2f(this->enemies[j].getPosition().x + 20.f,
											this->enemies[j].getPosition().y - 20.f),
										Vector2f(1.f, 0.f),
										28,
										20.f,
										true
									)
								);
							}

							//Enemy dead
							if (this->enemies[j].getHP() <= 0)
							{
								//GAIN EXP
								int exp = this->enemies[j].getHPMax()
									+ (rand() % this->enemies[j].getHPMax() + 1);

								//GAIN SCORE
								int score = this->enemies[j].getHPMax();
								this->players[i].gainScore(score);

								//LEVEL UP TAG
								if (this->players[i].gainExp(exp))
								{
									//Create text tag
									this->textTags.add(
										TextTag(&this->font,
											"LEVEL UP!",
											Color::Cyan,
											Vector2f(this->players[i].getPosition().x + 20.f,
												this->players[i].getPosition().y - 20.f),
											Vector2f(0.f, 1.f),
											32,
											30.f,
											true
										)
									);
								}

								this->enemies.remove(j);

								//GAIN EXP TAG
								//Create text tag
								this->textTags.add(
									TextTag(&this->font,
										"+" + std::to_string(exp) + " exp",
										Color::Cyan,
										Vector2f(this->players[i].getPosition().x + 20.f,
											this->players[i].getPosition().y - 20.f),
										Vector2f(0.f, 1.f),
										24,
										25.f,
										true
									)
								);
							}

							return;	//RETURN!!!
						}
					}

					//Window bounds check
					if (this->players[i].getBullet(k).getPosition().x > this->window->getSize().x)
					{
						this->players[i].removeBullet(k);
						return;	//RETURN!!!
					}
				}
			}
		}

		//Update enemies
		for (size_t i = 0; i < this->enemies.size(); i++)
		{
			this->enemies[i].Update(dt, this->players[this->enemies[i].getPlayerFollowNr()].getPosition());

			//Enemy player collision
			for (size_t k = 0; k < this->players.size(); k++)
			{
				if (this->players[k].isAlive())
				{
					if (this->players[k].getGlobalBounds().intersects(this->enemies[i].getGlobalBounds())
						&& !this->players[k].isDamageCooldown())
					{
						int damage = this->enemies[i].getDamage();
						
						this->players[k].takeDamage(damage);
						
						//Create text tag
						this->textTags.add(
							TextTag(&this->font,
								"-" + std::to_string(damage),
								Color::Red,
								Vector2f(this->players[k].getPosition().x + 20.f,
									this->players[k].getPosition().y - 20.f),
								Vector2f(-1.f, 0.f),
								30, 
								20.f,
								true
							)
						);
						
						//Player death
						if (!this->players[k].isAlive())
							this->playersAlive--;

						return;//RETURN!!!
					}
				}
			}

			//Enemies out of bounds
			if (this->enemies[i].getPosition().x < 0 - this->enemies[i].getGlobalBounds().width)
			{
				this->enemies.remove(i);
				return;	//RETURN!!!
			}
		}

		//Update Texttags
		for (size_t i = 0; i < this->textTags.size(); i++)
		{
			this->textTags[i].Update(dt);

			if (this->textTags[i].getTimer() <= 0.f)
			{
				this->textTags.remove(i);
				break;
			}
		}
	}
}

void Game::DrawUI()
{

}

void Game::Draw()
{
	this->window->clear();

	//Draw player
	for (size_t i = 0; i < this->players.size(); i++)
	{
		if (this->players[i].isAlive())
		{			
			this->players[i].Draw(*this->window);

			//UI
			this->UpdateUIPlayer(i);
			this->window->draw(this->followPlayerText);
			this->window->draw(this->playerExpBar);
		}
	}

	//Draw enemies
	for (size_t i = 0; i < this->enemies.size(); i++)
	{
		this->enemies[i].Draw(*this->window);

		//UI
		this->UpdateUIEnemy(i);
		this->window->draw(this->enemyText);
	}

	//Draw texttags
	for (size_t i = 0; i < this->textTags.size(); i++)
	{
		this->textTags[i].Draw(*this->window);
	}

	//GAME OVER TEXT
	if (this->playersAlive <= 0)
	{
		this->window->draw(this->gameOverText);
	}

	this->window->display();
}