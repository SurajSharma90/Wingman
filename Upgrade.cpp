#include "Upgrade.h"

Upgrade::Upgrade(
	dArr<Texture> &textures,
	Vector2f position,
	int type,
	float aliveTimerMax
)
{
	this->dtMultiplier = 62.5f;

	this->aliveTimerMax = aliveTimerMax;

	this->type = type;

	this->textures = &textures;
	
	if (this->type < (*this->textures).size())
		this->sprite.setTexture((*this->textures)[this->type]);
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