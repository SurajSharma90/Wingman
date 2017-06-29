#include "Player.h"

unsigned Player::players = 0;

enum controls {UP = 0, DOWN, LEFT, RIGHT, SHOOT};
enum weapons {LASER = 0, MISSILE01, MISSILE02};

Player::Player(
	std::vector<Texture> &textures,
	dArr<Texture> &lWingTextures,
	dArr<Texture> &rWingTextures,
	dArr<Texture> &cPitTextures,
	dArr<Texture> &auraTextures,
	int UP, int DOWN,
	int LEFT, int RIGHT,
	int SHOOT)

	:level(1), exp(0),
	hp(10), hpMax(10),
	statPoints(0), cooling(0), 
	plating(0), wiring(0), power(0),
	damage(1), damageMax(2),
	score(0)
{
	//Dt
	this->dtMultiplier = 62.5f;

	//Keytime
	this->keyTimeMax = 8.f;
	this->keyTime = this->keyTimeMax;

	//Stats
	this->expNext = 20 + static_cast<int>(
		(50 / 3)
		*((pow(level, 3) - 6 
			* pow(level, 2)) + 17 
			* level - 12)
		);

	//Update positions
	this->playerCenter.x = this->sprite.getPosition().x +
		this->sprite.getGlobalBounds().width / 2;
	this->playerCenter.y = this->sprite.getPosition().y +
		this->sprite.getGlobalBounds().height / 2;

	//Textures & Sprites
	this->lWingTextures = &lWingTextures;
	this->rWingTextures = &rWingTextures;
	this->cPitTextures = &cPitTextures;
	this->auraTextures = &auraTextures;

	this->sprite.setTexture(textures[0]);
	this->sprite.setScale(0.09f, 0.09f);
	this->sprite.setColor(Color(10, 10, 10, 255));

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

	//Accessories

	//Selectors
	this->lWingSelect = 1;
	this->rWingSelect = 1;
	this->cPitSelect = 1;
	this->auraSelect = 1;

	//Acceccory textures
	this->lWing.setTexture((*this->lWingTextures)[this->lWingSelect]);
	this->rWing.setTexture((*this->rWingTextures)[this->rWingSelect]);
	this->cPit.setTexture((*this->cPitTextures)[this->cPitSelect]);
	this->aura.setTexture((*this->auraTextures)[this->auraSelect]);

	//Init accessories
	this->lWing.setOrigin(
		this->lWing.getGlobalBounds().width / 2,
		this->lWing.getGlobalBounds().height / 2
		);

	this->lWing.setPosition(this->playerCenter);
	this->lWing.setRotation(90.f);
	this->lWing.setScale(0.8f, 0.8f);

	this->rWing.setOrigin(
		this->rWing.getGlobalBounds().width / 2,
		this->rWing.getGlobalBounds().height / 2
	);

	this->rWing.setPosition(this->playerCenter);
	this->rWing.setRotation(90.f);
	this->rWing.setScale(0.8f, 0.8f);

	this->cPit.setOrigin(
		this->cPit.getGlobalBounds().width / 2,
		this->cPit.getGlobalBounds().height / 2
		);

	this->cPit.setPosition(this->playerCenter);
	this->cPit.setRotation(90.f);
	this->cPit.setScale(0.8f, 0.8f);

	this->aura.setOrigin(
		this->aura.getGlobalBounds().width / 2,
		this->aura.getGlobalBounds().height / 2
		);

	this->aura.setPosition(this->playerCenter);
	this->aura.setRotation(90.f);
	this->aura.setScale(0.8f, 0.8f);

	//Timers
	this->shootTimerMax = 25.f;
	this->shootTimer = this->shootTimerMax;
	this->damageTimerMax = 40.f;
	this->damageTimer = this->damageTimerMax;

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

int Player::getDamage()const
{ 
	int damage = 0;

	switch (this->currentWeapon)
	{
	case LASER:

		damage = rand() % this->damageMax + this->damage;

		break;
	case MISSILE01:

		damage = rand() % this->damageMax + this->damage;
		damage *= 2;

		break;
	case MISSILE02:

		damage = rand() % this->damageMax + this->damage;
		damage *= 4;

		break;
	default:

		damage = rand() % this->damageMax + this->damage;

		break;
	}

	return damage;
}

void Player::takeDamage(int damage)
{ 
	this->hp -= damage; 

	this->damageTimer = 0;

	this->currentVelocity.x += -this->normDir.x*10.f;
	this->currentVelocity.y += -this->normDir.y*10.f;
}

bool Player::UpdateLeveling()
{
	if (this->exp >= this->expNext)
	{
		this->level++;
		this->statPoints++;
		this->exp -= this->expNext;
		this->expNext = static_cast<int>((50 / 3)*((pow(level, 3) - 6 * pow(level, 2)) + 17 * level - 12));

		this->wiring++;
		this->cooling++;
		this->plating++;
		this->power++;

		this->hpMax = 10 + plating * 5;
		this->damageMax = 2 + power*2;
		this->damage = 1 + power;

		this->hp = hpMax;

		return true;
	}

	return false;
}

void Player::ChangeAccessories()
{
	if (Keyboard::isKeyPressed(Keyboard::Num1) && this->keyTime >= this->keyTimeMax)
	{
		if (lWingSelect < (*this->lWingTextures).size() - 1)
			lWingSelect++;
		else
			lWingSelect = 0;

		this->lWing.setTexture((*this->lWingTextures)[lWingSelect]);

		this->keyTime = 0;
	}

	if (Keyboard::isKeyPressed(Keyboard::Num2) && this->keyTime >= this->keyTimeMax)
	{
		if (rWingSelect < (*this->rWingTextures).size() - 1)
			rWingSelect++;
		else
			rWingSelect = 0;

		this->rWing.setTexture((*this->rWingTextures)[rWingSelect]);

		this->keyTime = 0;
	}

	if (Keyboard::isKeyPressed(Keyboard::Num3) && this->keyTime >= this->keyTimeMax)
	{
		if (cPitSelect < (*this->cPitTextures).size() - 1)
			cPitSelect++;
		else
			cPitSelect = 0;

		this->cPit.setTexture((*this->cPitTextures)[cPitSelect]);

		this->keyTime = 0;
	}

	if (Keyboard::isKeyPressed(Keyboard::Num4) && this->keyTime >= this->keyTimeMax)
	{
		if (auraSelect < (*this->auraTextures).size() - 1)
			auraSelect++;
		else
			auraSelect = 0;

		this->aura.setTexture((*this->auraTextures)[auraSelect]);

		this->keyTime = 0;
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

	//Left wing
	this->lWing.setPosition(
		playerCenter.x + -abs(this->currentVelocity.x), 
		playerCenter.y + -abs(this->currentVelocity.x/2 + this->currentVelocity.y/2)
	);

	//Right wing
	this->rWing.setPosition(
		playerCenter.x + -abs(this->currentVelocity.x), 
		playerCenter.y + abs(this->currentVelocity.x / 2 + this->currentVelocity.y / 2)
	);

	//Cockpit
	this->cPit.setPosition(
		playerCenter.x + this->currentVelocity.x, 
		playerCenter.y
	);

	//Aura
	this->aura.setPosition(playerCenter);
	this->aura.rotate(3.f * dt * this->dtMultiplier);
}

void Player::Movement(Vector2u windowBounds, const float &dt)
{
	//Update normalized direction
	this->normDir = normalize(this->currentVelocity, vectorLength(this->currentVelocity));

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

	//Window collision
	//left
	if (this->getPosition().x <= 0)
	{
		this->sprite.setPosition(0.f, this->sprite.getPosition().y);
		this->currentVelocity.x = 0.f;
	}
	else if (this->getPosition().y <= 0)
	{
		this->sprite.setPosition(this->sprite.getPosition().x, 0.f);
		this->currentVelocity.y = 0.f;
	}
	else if (this->getPosition().x + this->getGlobalBounds().width >= windowBounds.x)
	{
		this->sprite.setPosition(windowBounds.x - this->getGlobalBounds().width, this->sprite.getPosition().y);
		this->currentVelocity.x = 0.f;
	}
	else if (this->getPosition().y + this->getGlobalBounds().height >= windowBounds.y)
	{
		this->sprite.setPosition(this->sprite.getPosition().x, windowBounds.y - this->getGlobalBounds().height);
		this->currentVelocity.y = 0.f;
	}
		
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
				this->bullets.add(
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
			this->bullets.add(
				Bullet(missile01Texture,
					Vector2f(this->playerCenter.x, this->playerCenter.y - 25.f),
					Vector2f(0.05f, 0.05f),
					Vector2f(1.f, 0.f), 
					0.f, 50.f, 1.f));

			if (this->dualMissiles01)
			{
				this->bullets.add(
					Bullet(missile01Texture,
						Vector2f(this->playerCenter.x, this->playerCenter.y + 25.f),
						Vector2f(0.05f, 0.05f),
						Vector2f(1.f, 0.f),
						0.f, 50.f, 1.f));
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

	//DAMAGED
	if (this->isDamageCooldown())
	{
		if ((int)this->damageTimer % 2 == 0)
		{
			this->lWing.setColor(Color::Red);
			this->rWing.setColor(Color::Red);
			this->cPit.setColor(Color::Red);
		}
		else
		{
			this->lWing.setColor(Color::White);
			this->rWing.setColor(Color::White);
			this->cPit.setColor(Color::White);
		}
	}
	else
	{
		this->lWing.setColor(Color::White);
		this->rWing.setColor(Color::White);
		this->cPit.setColor(Color::White);
	}
}

Bullet& Player::getBullet(unsigned index)
{
	if (index < 0 || index > this->bullets.size())
		throw"OUT OF BOUNDS! PLAYER::GETBULLET!";

	return this->bullets[index];
}

void Player::removeBullet(unsigned index)
{
	if (index < 0 || index > this->bullets.size())
		throw"OUT OF BOUNDS! PLAYER::REMOVEBULLET!";

	this->bullets.remove(index);
}

void Player::Update(Vector2u windowBounds, const float &dt)
{
	//Update timers
	if (this->shootTimer < this->shootTimerMax)
		this->shootTimer += 1.f * dt * this->dtMultiplier;

	if (this->damageTimer < this->damageTimerMax)
		this->damageTimer += 1.f * dt * this->dtMultiplier;

	if (this->keyTime < this->keyTimeMax)
		this->keyTime += 1.f * dt * this->dtMultiplier;

	this->Movement(windowBounds, dt);
	this->ChangeAccessories();
	this->UpdateAccessories(dt);
	this->Combat(dt);
}

void Player::Draw(RenderTarget &target)
{
	target.draw(this->aura);

	for (size_t i = 0; i < this->bullets.size(); i++)
	{
		this->bullets[i].Draw(target);
	}

	target.draw(this->mainGunSprite);

	target.draw(this->sprite);

	target.draw(this->cPit);
	target.draw(this->lWing);
	target.draw(this->rWing);
}