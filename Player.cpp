#include "Player.h"

unsigned Player::players = 0;

enum controls {UP = 0, DOWN, LEFT, RIGHT, SHOOT};
enum weapons {LASER = 0, MISSILE01, MISSILE02};

Player::Player(
	std::vector<Texture> &textures,
	int UP, int DOWN,
	int LEFT, int RIGHT,
	int SHOOT)
	:level(1), exp(0),
	hp(10), hpMax(10),
	damage(1), damageMax(2),
	score(0)
{
	//Dt
	this->dtMultiplier = 62.5f;

	//Stats
	this->expNext = 20 + static_cast<int>(
		(50 / 3)
		*((pow(level, 3) - 6 
			* pow(level, 2)) + 17 
			* level - 12)
		);

	std::cout << this->expNext << "\n";

	//Update positions
	this->playerCenter.x = this->sprite.getPosition().x +
		this->sprite.getGlobalBounds().width / 2;
	this->playerCenter.y = this->sprite.getPosition().y +
		this->sprite.getGlobalBounds().height / 2;

	//Textures & Sprites
	this->sprite.setTexture(textures[0]);
	this->sprite.setScale(0.13f, 0.13f);

	this->laserTexture = &textures[1];
	this->missile01Texture = &textures[2];

	this->mainGunSprite.setTexture(textures[3]);
	this->mainGunSprite.setOrigin(
		this->mainGunSprite.getGlobalBounds().width / 2,
		this->mainGunSprite.getGlobalBounds().height / 2
	);

	this->mainGunSprite.rotate(90);

	this->mainGunSprite.setPosition(
		this->playerCenter.x + 20.f,
		this->playerCenter.y
	);

	//Timers
	this->shootTimerMax = 25.f;
	this->shootTimer = this->shootTimerMax;
	this->damageTimer = 10.f;
	this->damageTimer = this->damageTimer;

	//Controls
	this->controls[controls::UP] = UP;
	this->controls[controls::DOWN] = DOWN;
	this->controls[controls::LEFT] = LEFT;
	this->controls[controls::RIGHT] = RIGHT;
	this->controls[controls::SHOOT] = SHOOT;

	//Velocity & Acceleration
	this->maxVelocity = 25.f;
	this->acceleration = 0.8f;
	this->stabilizerForce = 0.4f;

	//Guns
	this->currentWeapon = LASER;

	//Upgrades
	this->mainGunLevel = 0;
	this->dualMissiles01 = false;
	this->dualMissiles02 = false;

	//Add number of players for coop
	this->playerNr = Player::players;
	Player::players++;
}

Player::~Player()
{

}

void Player::UpdateLeveling()
{
	if (this->exp >= this->expNext)
	{
		this->level++;
		this->statPoints++;
		this->exp -= this->expNext;
		this->expNext = static_cast<int>((50 / 3)*((pow(level, 3) - 6 * pow(level, 2)) + 17 * level - 12));
	}
}

void Player::UpdateAccessories(const float &dt)
{
	//Set the position of gun to follow player
	this->mainGunSprite.setPosition(
		this->mainGunSprite.getPosition().x,
		this->playerCenter.y
	);

	//Animate the main gun and correct it after firing
	if (this->mainGunSprite.getPosition().x < this->playerCenter.x + 20.f)
	{
		this->mainGunSprite.move(2.f * dt * this->dtMultiplier 
			+ this->currentVelocity.x * dt * this->dtMultiplier, 0.f);
	}
	if (this->mainGunSprite.getPosition().x > this->playerCenter.x + 20.f)
	{
		this->mainGunSprite.setPosition(
			this->playerCenter.x + 20.f,
			this->playerCenter.y
		);
	}
}

void Player::Movement(const float &dt)
{
	//UP
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[controls::UP])))
	{
		this->direction.x = 0.f;
		this->direction.y = -1.f;
	
		if (this->currentVelocity.y > -this->maxVelocity && this->direction.y < 0)
			this->currentVelocity.y += this->direction.y * this->acceleration
			* dt * this->dtMultiplier;
	}
	
	//DOWN
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[controls::DOWN])))
	{ 
		this->direction.x = 0.f;
		this->direction.y = 1.f;

		if (this->currentVelocity.y < this->maxVelocity && this->direction.y > 0)
			this->currentVelocity.y += this->direction.y * this->acceleration
			* dt * this->dtMultiplier;
	}
	
	//LEFT
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[controls::LEFT])))
	{ 
		this->direction.x = -1.f;
		this->direction.y = 0.f;

		if (this->currentVelocity.x > -this->maxVelocity && this->direction.x < 0)
			this->currentVelocity.x += this->direction.x * this->acceleration
			* dt * this->dtMultiplier;

	}
	
	//RIGHT
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[controls::RIGHT])))
	{ 
		this->direction.x = 1.f;
		this->direction.y = 0.f;

		if (this->currentVelocity.x < this->maxVelocity && this->direction.x > 0)
			this->currentVelocity.x += this->direction.x * this->acceleration
			* dt * this->dtMultiplier;

	}

	//Drag force
	if (this->currentVelocity.x > 0)
	{
		this->currentVelocity.x -= this->stabilizerForce
			* dt * this->dtMultiplier;

		if (this->currentVelocity.x < 0)
			this->currentVelocity.x = 0;
	}
	else if (this->currentVelocity.x < 0)
	{
		this->currentVelocity.x += this->stabilizerForce
			* dt * this->dtMultiplier;

		if (this->currentVelocity.x > 0)
			this->currentVelocity.x = 0;
	}
	if (this->currentVelocity.y > 0)
	{
		this->currentVelocity.y -= this->stabilizerForce
			* dt * this->dtMultiplier;

		if (this->currentVelocity.y < 0)
			this->currentVelocity.y = 0;
	}
	else if (this->currentVelocity.y < 0)
	{
		this->currentVelocity.y += this->stabilizerForce
			* dt * this->dtMultiplier;

		if (this->currentVelocity.y > 0)
			this->currentVelocity.y = 0;
	}

	//Final move
	this->sprite.move(this->currentVelocity.x* dt * this->dtMultiplier, 
		this->currentVelocity.y * dt * this->dtMultiplier);

	//Update positions
	this->playerCenter.x = this->sprite.getPosition().x +
		this->sprite.getGlobalBounds().width / 2;
	this->playerCenter.y = this->sprite.getPosition().y +
		this->sprite.getGlobalBounds().height / 2;
}

void Player::Combat(const float &dt)
{
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[controls::SHOOT]))
		&& this->shootTimer >= this->shootTimerMax)
	{
		if (this->currentWeapon == LASER)
		{
			//Create bullet
			if (this->mainGunLevel == 0)
			{
				this->bullets.push_back(
					Bullet(laserTexture,
						Vector2f(this->playerCenter.x + 50.f, this->playerCenter.y),
						Vector2f(0.2f, 0.2f),
						Vector2f(1.f, 0.f),
						20.f, 60.f, 5.f));
			}
			else if (this->mainGunLevel == 1)
			{

			}
			else if (this->mainGunLevel == 2)
			{

			}

			//Animate gun
			this->mainGunSprite.move(-30.f, 0.f);
		}
		else if (this->currentWeapon == MISSILE01)
		{
			//Create bullet
			this->bullets.push_back(
				Bullet(missile01Texture,
					Vector2f(this->playerCenter.x, this->playerCenter.y - 25.f),
					Vector2f(0.05f, 0.05f),
					Vector2f(1.f, 0.f), 
					2.f, 50.f, 1.f));

			if (this->dualMissiles01)
			{
				this->bullets.push_back(
					Bullet(missile01Texture,
						Vector2f(this->playerCenter.x, this->playerCenter.y + 25.f),
						Vector2f(0.05f, 0.05f),
						Vector2f(1.f, 0.f),
						2.f, 50.f, 1.f));
			}
		}
		else if (this->currentWeapon == MISSILE02)
		{
			if (this->dualMissiles02)
			{
				
			}
		}
	
		this->shootTimer = 0; //RESET TIMER!
	}
}

void Player::Update(Vector2u windowBounds, const float &dt)
{
	//Update timers
	if (this->shootTimer < this->shootTimerMax)
		this->shootTimer += 1.f * dt * this->dtMultiplier;

	if (this->damageTimer < this->damageTimerMax)
		this->damageTimer += 1.f * dt * this->dtMultiplier;

	this->Movement(dt);
	this->UpdateAccessories(dt);
	this->Combat(dt);
}

void Player::Draw(RenderTarget &target)
{
	for (size_t i = 0; i < this->bullets.size(); i++)
	{
		this->bullets[i].Draw(target);
	}

	target.draw(this->mainGunSprite);

	target.draw(this->sprite);
}