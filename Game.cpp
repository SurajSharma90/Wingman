#include "Game.h"

enum texture { player = 0, laser01, missile01 };

Game::Game(RenderWindow *window)
{
	this->window = window;
	this->window->setFramerateLimit(300);
	this->fullscreen = false;
	this->dtMultiplier = 62.5f;
	this->scoreMultiplier = 1;
	this->score = 0;
	this->scoreTimer.restart();
	this->scoreTime = 0;
	this->multiplierAdderMax = 10;
	this->multiplierAdder = 0;
	this->multiplierTimerMax = 200.f;
	this->multiplierTimer = this->multiplierTimerMax;
	this->bestScoreSecond = 0.0;
	this->difficulty = 0;
	this->difficultyTimer = 0.f;

	this->paused = true;
	this->keyTimeMax = 10.f;
	this->keyTime = this->keyTimeMax;

	//Init fonts
	this->font.loadFromFile("Fonts/Dosis-Light.ttf");

	//INit textures
	this->initTextures();

	//Init map
	this->initMap();

	//Init players
	this->players.add(Player());

	this->players.add(Player(
		Keyboard::Numpad5, 
		Keyboard::Numpad4, 
		Keyboard::Numpad6, 
		Keyboard::RControl, 
		Keyboard::Numpad7,
		Keyboard::Numpad1,
		Keyboard::Numpad2,
		Keyboard::Numpad3,
		Keyboard::Numpad0));
	
	this->playersAlive = this->players.size();

	//Init enemies

	this->enemySpawnTimerMax = 35.f; //ALSO IN RESET!
	this->enemySpawnTimer = this->enemySpawnTimerMax;

	//Init bosses
	this->bossEncounter = false;

	this->initUI();
}

Game::~Game()
{

}

void Game::toggleFullscreen()
{
	if (Keyboard::isKeyPressed(Keyboard::F11) && this->keyTime >= this->keyTimeMax)
	{
		this->keyTime = 0.f;

		if (fullscreen)
		{
			this->fullscreen = false;
			this->window->close();
			this->window->create(sf::VideoMode(1920, 1080), "Wingman Game", Style::Default);
		}
		else
		{
			this->fullscreen = true;
			this->window->close();
			this->window->create(sf::VideoMode(1920, 1080), "Wingman Game", Style::Fullscreen);
		}
	}
}

void Game::pauseGame()
{
	if (Keyboard::isKeyPressed(Keyboard::P) && this->keyTime >= this->keyTimeMax)
	{
		if (this->paused)
			this->paused = false;
		else
			this->paused = true;

		this->keyTime = 0.f;
	}
}

void Game::initPlayerTextures()
{
	Texture temp;

	//Init textures regular
	temp.loadFromFile("Textures/ship.png");
	Player::playerBodyTextures.add(temp);

	//Bullets
	temp.loadFromFile("Textures/Guns/rayTex01.png");
	Player::playerBulletTextures.add(temp);
	temp.loadFromFile("Textures/Guns/missileTex01.png");
	Player::playerBulletTextures.add(temp);

	//Player guns
	temp.loadFromFile("Textures/Guns/gun01.png");
	Player::playerMainGunTextures.add(Texture(temp));
	temp.loadFromFile("Textures/Guns/gun02.png");
	Player::playerMainGunTextures.add(Texture(temp));
	temp.loadFromFile("Textures/Guns/gun03.png");
	Player::playerMainGunTextures.add(Texture(temp));

	//Init accessory textures
	std::ifstream in;

	in.open("Textures/Accessories/leftwings.txt");
	std::string fileName = "";

	if (in.is_open())
	{
		while (getline(in, fileName))
		{
			temp.loadFromFile(fileName);
			Player::lWingTextures.add(Texture(temp));
		}
	}

	in.close();

	in.open("Textures/Accessories/rightwings.txt");
	fileName = "";

	if (in.is_open())
	{
		while (getline(in, fileName))
		{
			temp.loadFromFile(fileName);
			Player::rWingTextures.add(Texture(temp));
		}
	}

	in.close();

	in.open("Textures/Accessories/cockpits.txt");
	fileName = "";

	if (in.is_open())
	{
		while (getline(in, fileName))
		{
			temp.loadFromFile(fileName);
			Player::cPitTextures.add(Texture(temp));
		}
	}

	in.close();

	in.open("Textures/Accessories/auras.txt");
	fileName = "";

	if (in.is_open())
	{
		while (getline(in, fileName))
		{
			temp.loadFromFile(fileName);
			Player::auraTextures.add(Texture(temp));
		}
	}

	in.close();
}

void Game::initTextures()
{
	Texture temp;

	//Player
	this->initPlayerTextures();

	//Pickup textures
	temp.loadFromFile("Textures/Pickups/hpSupply.png");
	this->pickupTextures.add(Texture(temp));
	temp.loadFromFile("Textures/Pickups/missileSupply.png");
	this->pickupTextures.add(Texture(temp));
	temp.loadFromFile("Textures/Pickups/missileHSupply.png");
	this->pickupTextures.add(Texture(temp));
	this->nrOfPickups = this->pickupTextures.size();

	//Upgrades
	temp.loadFromFile("Textures/Upgrades/statpoint.png");
	this->upgradeTextures.add(Texture(temp));
	temp.loadFromFile("Textures/Upgrades/healthtank.png");
	this->upgradeTextures.add(Texture(temp));
	temp.loadFromFile("Textures/Upgrades/doubleray.png");
	this->upgradeTextures.add(Texture(temp));
	temp.loadFromFile("Textures/Upgrades/tripleray.png");
	this->upgradeTextures.add(Texture(temp));
	temp.loadFromFile("Textures/Upgrades/piercingshot.png");
	this->upgradeTextures.add(Texture(temp));
	temp.loadFromFile("Textures/Upgrades/shield.png");
	this->upgradeTextures.add(Texture(temp));
	this->nrOfUpgrades = this->upgradeTextures.size();
	
	//Enemies
	temp.loadFromFile("Textures/enemyMoveLeft.png");
	this->enemyTextures.add(Texture(temp));
	temp.loadFromFile("Textures/enemyFollow.png");
	this->enemyTextures.add(Texture(temp));
	temp.loadFromFile("Textures/enemyMoveLeftShoot.png");
	this->enemyTextures.add(Texture(temp));

	temp.loadFromFile("Textures/Guns/enemyBullet.png");
	this->enemyBulletTextures.add(Texture(temp));

	//Bosses
	temp.loadFromFile("Textures/Bosses/Bodies/BossBody01.png");
	this->bossBodyTextures.add(Texture(temp));

	temp.loadFromFile("Textures/Bosses/Guns/BossGun01.png");
	this->bossGunTextures.add(Texture(temp));

	temp.loadFromFile("Textures/Bosses/Bullets/BossBullet01.png");
	this->bossBulletTextures.add(Texture(temp));

}

void Game::initUI()
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

	this->scoreText.setFont(this->font);
	this->scoreText.setFillColor(Color(200,200,200,150));
	this->scoreText.setCharacterSize(32);
	this->scoreText.setString("Score: 0");
	this->scoreText.setPosition(10.f, 10.f);

	this->controlsText.setFont(this->font);
	this->gameOverText.setFillColor(Color::White);
	this->controlsText.setCharacterSize(32);
	this->controlsText.setString(
		"A: LEFT\nD: RIGHT\nW: UP\nS: DOWN\nSPACE: SHOOT\nP: PAUSE/CONTROLS (START GAME)\nESC: QUIT\n1,2,3 & 4: CUSTOMIZE SHIP (CAN DO WHILE PAUSED!)\nF11: FULLSCREEN\n\nTOP-LEFT SHIP: Player number\nTOP-RIGHT SHIP: Hp/HpMax\nBOTTOM-LEFT SHIP: Level\nBOTTOM-RIGHT SHIP: Exp-bar\n\nWARNING, SCORE-TIMER DOES NOT STOP WHEN PAUSED!"
	);
	this->controlsText.setPosition(50.f, 400.f);

	//PlayerStatText init
	this->playerStatsText.setFont(font);
	this->playerStatsText.setFillColor(Color::White);
	this->playerStatsText.setCharacterSize(16);
	this->playerStatsText.setString("NONE");

	this->playerStatsTextBack.setFillColor(Color(50, 50, 50, 100));
	this->playerStatsTextBack.setOutlineThickness(1.f);
	this->playerStatsTextBack.setOutlineColor(Color(255, 255, 255, 200));
}

void Game::initMap()
{
	RectangleShape temp;
	temp.setSize(Vector2f(100.f, 100.f));
	temp.setFillColor(Color::White);
	temp.setPosition(500.f, 500.f);

	this->walls.add(RectangleShape(temp));
	temp.setPosition(600.f, 500.f);
	this->walls.add(RectangleShape(temp));
	temp.setPosition(400.f, 500.f);
	this->walls.add(RectangleShape(temp));
	temp.setPosition(300.f, 500.f);
	this->walls.add(RectangleShape(temp));
}

void Game::restartUpdate()
{
	if (Keyboard::isKeyPressed(Keyboard::F1))
	{
		for (size_t i = 0; i < this->players.size(); i++)
		{
			this->players[i].reset();
		}

		this->playersAlive = this->players.size();
		this->score = 0;
		this->scoreMultiplier = 1;
		this->multiplierAdder = 0;
		this->scoreTime = 0;
		this->difficulty = 0;
		this->bossEncounter = false;
		this->enemySpawnTimerMax = 35.f; //ALSO IN CONSTUCTOR!
		this->scoreTimer.restart();
		this->enemies.clear();
		this->upgrades.clear();
		this->pickups.clear();
		this->bosses.clear();
	}
}

void Game::setEndingScoreboard()
{
	this->scoreTime = (int)this->scoreTimer.getElapsedTime().asSeconds();

	if (this->scoreTime == 0)
		this->scoreTime = 1;

	this->gameOverText.setString
	(
		std::string("GAME OVER\nScore: " +
			std::to_string(this->score) +
			"\n" +
			"Time: " +
			std::to_string(this->scoreTime) +
			"\n" +
			"Score/Second: " +
			std::to_string((int)round((double)this->score / (double)this->scoreTime))) +
		"\nF1 to RESTART"
	);

	if ((double)this->score / (double)this->scoreTime > this->bestScoreSecond)
		this->bestScoreSecond = (double)this->score / (double)this->scoreTime;
}

void Game::updateTimers(const float &dt)
{
	if (this->keyTime < this->keyTimeMax)
		this->keyTime += 1.f * dt * this->dtMultiplier;
}

void Game::updateTimersUnpaused(const float &dt)
{
	//Enemy spawn timer
	if (this->enemySpawnTimer < this->enemySpawnTimerMax)
		this->enemySpawnTimer += 1.f * dt * this->dtMultiplier;

	//Difficulty timer
	this->difficultyTimer += 1.f * dt * this->dtMultiplier;

	//Score and multipliers
	if (this->multiplierTimer > 0.f)
	{
		this->multiplierTimer -= 1.f * dt * this->dtMultiplier;

		if (this->multiplierTimer <= 0.f)
		{
			this->multiplierTimer = 0.f;
			this->multiplierAdder = 0;
			this->scoreMultiplier = 1;
		}
	}
}

void Game::updateScore()
{
	if (this->multiplierAdder >= this->multiplierAdderMax)
	{
		this->multiplierAdder = 0;
		this->scoreMultiplier++;
	}
}

void Game::updateDifficulty()
{
	if ((int)this->difficultyTimer % 1000 == 0)
	{
		if(this->enemySpawnTimerMax > 10)
			this->enemySpawnTimerMax--;

		this->difficulty++;
		this->difficultyTimer = 1.f;
	}
}

void Game::updateUIPlayer(int index)
{
	if (index < 0 || index >= this->players.size())
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

		//STATS BOX WITH TEXT
		if (this->players[index].playerShowStatsIsPressed())
		{
			this->playerStatsText.setString(this->players[index].getStatsAsString());

			this->playerStatsTextBack.setPosition(
				this->players[index].getPosition().x, 
				this->players[index].getPosition().y + 150.f
			);
			this->playerStatsTextBack.setSize(Vector2f(this->playerStatsText.getGlobalBounds().width, this->playerStatsText.getGlobalBounds().height));
			
			this->playerStatsText.setPosition(this->playerStatsTextBack.getPosition());
		}
	}
}

void Game::updateUIEnemy(int index)
{
	this->enemyText.setPosition(
		this->enemies[index].getPosition().x,
		this->enemies[index].getPosition().y
		- this->enemies[index].getGlobalBounds().height
	);

	this->enemyText.setString(
		std::to_string(this->enemies[index].getHP()) +
		"/" +
		std::to_string(this->enemies[index].getHPMax()));
}

void Game::updateWhilePaused(const float &dt)
{
	//Change accessories when paused
	if (this->paused)
	{
		for (size_t i = 0; i < this->players.size(); i++)
		{
			if (this->players[i].isAlive())
			{
				this->players[i].changeAccessories(dt);
			}
		}
	}
}

void Game::playerUpdate(const float &dt)
{
	for (size_t i = 0; i < this->players.size(); i++)
	{
		if (this->players[i].isAlive())
		{
			//UPDATE PLAYERS
			this->players[i].update(this->window->getSize(), dt);

			//Upgrades update

			//Wall collision update
			for (size_t k = 0; k < this->walls.size(); k++)
			{
				if (this->players[i].getGlobalBounds().intersects(this->walls[k].getGlobalBounds()))
				{
					while (this->players[i].getGlobalBounds().intersects(this->walls[k].getGlobalBounds()))
					{
						this->players[i].move(
							20.f * -1.f * this->players[i].getNormDir().x,
							20.f * -1.f * this->players[i].getNormDir().y
						);
					}

					this->players[i].resetVelocity();
				}
			}

			//Bullets update
			this->playerBulletUpdate(dt, i);

			//UPDATE SCORE
			this->score = 0;

			for (size_t k = 0; k < this->players.size(); k++)
			{
				this->score += players[k].getScore();
			}

			this->scoreText.setString(
				"Score: " +
				std::to_string(this->score) +
				"\nMultiplier:" +
				std::to_string(this->scoreMultiplier) + "x" +
				"\nMultiplier Timer:" +
				std::to_string((int)this->multiplierTimer) +
				"\nNew Multiplier: " +
				std::to_string(this->multiplierAdder) +
				" / " +
				std::to_string(this->multiplierAdderMax) +
				"\nGame time: " +
				std::to_string((int)this->scoreTimer.getElapsedTime().asSeconds()) +
				"\nDifficulty: " +
				std::to_string(this->difficulty) +
				"\nBest Score/Second: " +
				std::to_string((int)round(this->bestScoreSecond))
			);
		}
	}
}

void Game::playerBulletUpdate(const float &dt, const int i)
{
	for (size_t k = 0; k < this->players[i].getBulletsSize(); k++)
	{
		this->players[i].getBullet(k).Update(dt);

		//Enemy collision check
		for (size_t j = 0; j < this->enemies.size(); j++)
		{
			if (this->players[i].getBullet(k).getGlobalBounds().intersects(this->enemies[j].getGlobalBounds()))
			{
				//Piercning shot check/Remove bullet
				if (!this->players[i].getPiercingShot())
				{
					this->players[i].removeBullet(k);
				}
				else
				{
					this->players[i].getBullet(k).setPosition(
						Vector2f(this->enemies[j].getPosition().x + this->enemies[j].getGlobalBounds().width + this->players[i].getBullet(k).getGlobalBounds().width / 2 + 1.f,
							this->players[i].getBullet(k).getPosition().y)
					);
				}

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
							Vector2f(this->enemies[j].getPosition().x,
								this->enemies[j].getPosition().y - 30.f),
							Vector2f(1.f, 0.f),
							28,
							30.f,
							true
						)
					);
				}

				//Enemy dead
				if (this->enemies[j].getHP() <= 0)
				{
					//GAIN SCORE & RESET MULTIPLIER TIMER
					this->multiplierTimer = this->multiplierTimerMax;
					int score = this->enemies[j].getHPMax() * this->scoreMultiplier;
					this->multiplierAdder++;
					this->players[i].gainScore(score);

					//GAIN EXP
					int exp = this->enemies[j].getHPMax()
						+ (rand() % this->enemies[j].getHPMax() + 1)
						* this->scoreMultiplier;

					//SCORE TEXT TAG
					this->textTags.add(
						TextTag(&this->font,
							"+ " + std::to_string(score) +
							"	( x" + std::to_string(this->scoreMultiplier) + " )",
							Color::White,
							Vector2f(100.f, 10.f),
							Vector2f(1.f, 0.f),
							30,
							40.f,
							true
						)
					);

					//LEVEL UP TAG
					if (this->players[i].gainExp(exp))
					{
						this->textTags.add(
							TextTag(&this->font,
								"LEVEL UP!",
								Color::Cyan,
								Vector2f(this->players[i].getPosition().x + 20.f,
									this->players[i].getPosition().y - 20.f),
								Vector2f(0.f, 1.f),
								32,
								40.f,
								true
							)
						);
					}

					//GAIN EXP TAG
					this->textTags.add(
						TextTag(&this->font,
							"+" +
							std::to_string(exp) +
							" ( x" +
							std::to_string(this->scoreMultiplier) +
							" ) " +
							" EXP",
							Color::Cyan,
							this->enemies[j].getPosition(),
							Vector2f(1.f, 0.f),
							24,
							40.f,
							true
						)
					);

					//ADD UPGRADE
					int dropChance = rand() % 100 + 1;
					int uType = 0;

					if (dropChance > 70)
					{
						//ADD PICKUP
						dropChance = rand() % 100 + 1;

						if (dropChance > 10)
							this->pickups.add(Pickup(
								this->pickupTextures,
								this->enemies[j].getPosition(),
								0,
								150.f)
							);
					}
					else
					{
						//ADD UPGRADE
						dropChance = rand() % 100 + 1;

						if (dropChance > 80)
						{
							uType = rand() % this->nrOfUpgrades;

							for (size_t k = 0; k < this->players[i].getAcquiredUpgrades().size(); k++)
							{
								if (uType == this->players[i].getAcquiredUpgrades()[k])
								{
									uType = rand() % 1;
								}
							}

							this->upgrades.add(Upgrade(
								this->upgradeTextures,
								this->enemies[j].getPosition(),
								uType,
								500.f)
							);
						}
					}

					this->enemies.remove(j);
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

void Game::enemyUpdate(const float &dt)
{
	//Spawn enemies
	if (this->enemySpawnTimer >= this->enemySpawnTimerMax)
	{
		this->enemies.add(Enemy(
			this->enemyTextures,
			this->enemyBulletTextures,
			this->window->getSize(),
			Vector2f(0.f, 0.f),
			Vector2f(-1.f, 0.f),
			rand() % 3,
			this->players[(rand() % playersAlive)].getLevel(),
			rand() % this->playersAlive)
		);

		this->enemySpawnTimer = 0; //Reset timer
	}

	//Update enemies
	for (size_t i = 0; i < this->enemies.size(); i++)
	{
		this->enemies[i].Update(dt, this->players[this->enemies[i].getPlayerFollowNr()].getPosition());

		//Enemy bullet update
		for (size_t k = 0; k < this->enemies[i].getBullets().size(); k++)
		{
			this->enemies[i].getBullets()[k].Update(dt);
		}

		//Enemy player collision
		for (size_t k = 0; k < this->players.size(); k++)
		{
			if (this->players[k].isAlive())
			{
				if (this->players[k].getGlobalBounds().intersects(this->enemies[i].getGlobalBounds())
					&& !this->players[k].isDamageCooldown())
				{
					int damage = this->enemies[i].getDamage();

					//PLAYER TAKES COLLISION DAMAGE
					this->players[k].takeDamage(damage);

					this->enemies[i].collision();

					//PLAYER TAKE DAMAGE TAG
					this->textTags.add(
						TextTag(&this->font,
							"-" + std::to_string(damage),
							Color::Red,
							Vector2f(this->players[k].getPosition().x + 20.f,
								this->players[k].getPosition().y - 20.f),
							Vector2f(-1.f, 0.f),
							30,
							30.f,
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
}

void Game::enemyBulletUpdate(const float &dt)
{

}

void Game::textTagsUpdate(const float &dt)
{
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

void Game::pickupsUpdate(const float &dt)
{
	for (size_t i = 0; i < this->pickups.size(); i++)
	{
		this->pickups[i].Update(dt);

		for (size_t k = 0; k < this->players.size(); k++)
		{
			if (this->pickups[i].checkCollision(this->players[k].getGlobalBounds()))
			{
				int gainHp = this->players[k].getHpMax() / 5;

				switch (this->pickups[i].getType())
				{
				case 0: //HP

					if (this->players[k].getHp() < this->players[k].getHpMax())
					{
						this->players[k].gainHP(gainHp);

						//GAIN HP TAG
						this->textTags.add(
							TextTag(&this->font,
								"+" +
								std::to_string(gainHp) +
								" HP",
								Color::Green,
								Vector2f(this->players[k].getPosition()),
								Vector2f(0.f, -1.f),
								24,
								40.f,
								true
							)
						);
					}
					else
					{
						//GAIN EXP TAG
						this->textTags.add(
							TextTag(&this->font,
								"+" +
								std::to_string(10) +
								" EXP",
								Color::Cyan,
								this->pickups[i].getPosition(),
								Vector2f(1.f, 0.f),
								24,
								40.f,
								true
							)
						);
						if (this->players[k].gainExp(10))
						{
							this->textTags.add(
								TextTag(&this->font,
									"LEVEL UP!",
									Color::Cyan,
									Vector2f(this->players[i].getPosition().x + 20.f,
										this->players[i].getPosition().y - 20.f),
									Vector2f(0.f, 1.f),
									32,
									40.f,
									true
								)
							);
						}
					}
					break;

				case 1: //MISSILE

					break;

				case 2: //HMISSILE

					break;

				default:

					break;
				}

				this->pickups.remove(i);
				return;
			}
		}
		if (this->pickups[i].canDelete())
		{
			this->pickups.remove(i);
			break;
		}
	}
}

void Game::upgradesUpdate(const float &dt)
{
	for (size_t i = 0; i < this->upgrades.size(); i++)
	{
		this->upgrades[i].Update(dt);

		for (size_t k = 0; k < this->players.size(); k++)
		{
			if (this->upgrades[i].checkCollision(this->players[k].getGlobalBounds()))
			{
				if (this->upgrades[i].getType() != 0 && this->upgrades[i].getType() != 1)
					this->players[k].getAcquiredUpgrades().add(this->upgrades[i].getType());

				switch (this->upgrades[i].getType())
				{

				case 0: //Statpoint

					this->players[k].addStatPointRandom();

					//HT TEXT TAG
					this->textTags.add(
						TextTag(&this->font,
							"RANDOM STATPOINT UPGRADE",
							Color::Yellow,
							this->players[k].getPosition(),
							Vector2f(1.f, 0.f),
							40,
							100.f,
							true
						)
					);

					break;

				case 1: //Healthtank
					this->players[k].upgradeHP();

					//HT TEXT TAG
					this->textTags.add(
						TextTag(&this->font,
							"PERMANENT HEALTH UPGRADE",
							Color::Yellow,
							this->players[k].getPosition(),
							Vector2f(1.f, 0.f),
							40,
							100.f,
							true
						)
					);

					break;

				case 2: //Doubleray
					if (this->players[k].getGunLevel() < 1)
						this->players[k].setGunLevel(1);

					//DR TEXT TAG
					this->textTags.add(
						TextTag(&this->font,
							"DOUBLE RAY UPGRADE",
							Color::Yellow,
							this->players[k].getPosition(),
							Vector2f(1.f, 0.f),
							40,
							100.f,
							true
						)
					);

					break;

				case 3: //Tripleray
					if (this->players[k].getGunLevel() < 2)
						this->players[k].setGunLevel(2);

					//TR TEXT TAG
					this->textTags.add(
						TextTag(&this->font,
							"TRIPLE RAY UPGRADE",
							Color::Yellow,
							this->players[k].getPosition(),
							Vector2f(1.f, 0.f),
							40,
							100.f,
							true
						)
					);

					break;

				case 4: //Piercing
					this->players[k].enablePiercingShot();

					//PS TEXT TAG
					this->textTags.add(
						TextTag(&this->font,
							"PIERCING SHOT UPGRADE",
							Color::Yellow,
							this->players[k].getPosition(),
							Vector2f(1.f, 0.f),
							40,
							100.f,
							true
						)
					);

					break;

				case 5: //Shield
					this->players[k].enableShield();

					//SH TEXT TAG
					this->textTags.add(
						TextTag(&this->font,
							"SHIELD UPGRADE",
							Color::Yellow,
							this->players[k].getPosition(),
							Vector2f(1.f, 0.f),
							40,
							100.f,
							true
						)
					);

					break;

				default:
					break;
				}

				this->upgrades.remove(i);
				return;
			}
		}

		if (this->upgrades[i].canDelete())
		{
			this->upgrades.remove(i);
			break;
		}
	}
}

void Game::update(const float &dt)
{	
	//Timers UPDATE
	this->updateTimers(dt);

	//Fullscreen
	this->toggleFullscreen();

	//PAUSE GAME
	this->pauseGame();

	//Update while game paused
	this->updateWhilePaused(dt);

	//GAME START
	if (this->playersAlive > 0 && !this->paused)
	{
		//Update timers
		this->updateTimersUnpaused(dt);

		//MAKE GAME HARDER WITH TIME
		this->updateDifficulty();

		//Score timer and multipliers
		this->updateScore();
		
		//Update players, bullets and combat
		this->playerUpdate(dt);

		//Update enemies
		this->enemyUpdate(dt);

		//Update Texttags
		this->textTagsUpdate(dt);

		//Upgrades update
		this->upgradesUpdate(dt);

		//Pickups update
		this->pickupsUpdate(dt);
	}
	else if(this->playersAlive <= 0 && this->scoreTime == 0)
	{
		this->setEndingScoreboard(); //Best score is set
	}
	
	//Restart if all players dead
	if (this->playersAlive <= 0)
	{
		this->restartUpdate();
	}
}

void Game::drawUI()
{
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

	//Score text
	this->window->draw(this->scoreText);

	//CONTROLS TEXT
	if (this->paused)
		this->window->draw(this->controlsText);
}

void Game::draw()
{
	this->window->clear();

	//Draw player
	for (size_t i = 0; i < this->players.size(); i++)
	{
		if (this->players[i].isAlive())
		{			
			this->players[i].draw(*this->window);

			//UI
			this->updateUIPlayer(i);
			
			this->window->draw(this->followPlayerText);
			this->window->draw(this->playerExpBar);
			
			if (this->players[i].playerShowStatsIsPressed())
			{
				this->window->draw(this->playerStatsTextBack);
				this->window->draw(this->playerStatsText);
			}
		}
	}

	//Draw enemies
	for (size_t i = 0; i < this->enemies.size(); i++)
	{
		this->enemies[i].Draw(*this->window);

		//UI
		this->updateUIEnemy(i);
		this->window->draw(this->enemyText);
	}

	//MAP
	for (size_t i = 0; i < this->walls.size(); i++)
	{
		this->window->draw(this->walls[i]);
	}

	//Draw pickups
	for (size_t i = 0; i < this->pickups.size(); i++)
	{
		this->pickups[i].Draw(*this->window);
	}

	//Draw Upgrades
	for (size_t i = 0; i < this->upgrades.size(); i++)
	{
		this->upgrades[i].Draw(*this->window);
	}

	this->drawUI();

	this->window->display();
}