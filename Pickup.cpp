#include "Pickup.h"

Pickup::Pickup(
	dArr<Texture> &textures,
	Vector2f position,
	int type,
	float duration)
{
	this->dtMultiplier = 62.5f;

	this->textures = &textures;

	this->type = type;
	if (this->type < (*this->textures).size())
		this->sprite.setTexture((*this->textures)[this->type]);
	else
		std::cout << "ERROR! NO TEXTURE FOR THIS TYPE! PICKUP." << "\n";
	
	this->sprite.setOrigin(
		this->sprite.getGlobalBounds().width / 2,
		this->sprite.getGlobalBounds().height / 2
	);

	this->sprite.setPosition(position);

	this->sprite.setColor(Color(255, 255, 255, 200));
	
	this->aliveTimerMax = duration;
	this->aliveTimer = 0;
}

Pickup::~Pickup()
{

}

bool Pickup::checkCollision(FloatRect rect)
{
	if (this->sprite.getGlobalBounds().intersects(rect))
		return true;

	return false;
}

void Pickup::Update(const float &dt)
{
	if (this->aliveTimer < this->aliveTimerMax)
		this->aliveTimer += 1.f * dt * this->dtMultiplier;

	this->sprite.rotate(5.f * dt * this->dtMultiplier);
}

void Pickup::Draw(RenderTarget &target)
{
	target.draw(this->sprite);
}
