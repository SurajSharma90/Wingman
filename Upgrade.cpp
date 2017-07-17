#include "Upgrade.h"

int Upgrade::nrOfUpgrades;
dArr<Texture> Upgrade::upgradeTextures;

Upgrade::Upgrade(
	Vector2f position,
	int type,
	float aliveTimerMax
)
{
	this->dtMultiplier = 62.5f;

	this->aliveTimerMax = aliveTimerMax;
	this->aliveTimer = 0;

	this->type = type;

	if (this->type < Upgrade::upgradeTextures.size())
		this->sprite.setTexture(Upgrade::upgradeTextures[this->type]);
	else
		std::cout << "NO TEXTURE FOR THAT UPGRADE TYPE!" << "\n";

	this->sprite.setOrigin
	(
		this->sprite.getGlobalBounds().width / 2,
		this->sprite.getGlobalBounds().height / 2
	);

	this->sprite.setPosition(position);
}

Upgrade::~Upgrade()
{

}

bool Upgrade::checkCollision(FloatRect rect)
{
	if (this->sprite.getGlobalBounds().intersects(rect))
		return true;

	return false;
}

void Upgrade::Update(const float &dt)
{
	if (this->aliveTimer < this->aliveTimerMax)
		this->aliveTimer += 1.f * dt * this->dtMultiplier;

	this->sprite.rotate(5.f * dt * this->dtMultiplier);
}

void Upgrade::Draw(RenderTarget &target)
{
	target.draw(this->sprite);
}