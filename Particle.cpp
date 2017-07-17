#include "Particle.h"

dArr<Texture> Particle::particleTextures;

Particle::Particle(
	Vector2f pos,
	int textureIndex,
	Vector2f dir,
	float maxVel,
	float maxRotation,
	float lifeTime,
	Color color
)
{
	this->dtMultiplier = 62.5f;

	this->sprite.setTexture(Particle::particleTextures[textureIndex]);
	this->sprite.setOrigin(
		this->sprite.getGlobalBounds().width / 2,
		this->sprite.getGlobalBounds().height / 2
		);
	this->sprite.setPosition(pos);
	this->sprite.setScale(Vector2f(rand() % 2 + 1, rand() % 2 + 1));
	this->sprite.setColor(color);
	this->color = color;

	this->dir.x = rand() % 10 + static_cast<int>(dir.x);
	this->dir.y = rand() % 10 + static_cast<int>(dir.y);

	this->dir = normalize(this->dir, vectorLength(this->dir));

	this->deceleration = 0.5f;
	this->maxVel = maxVel;
	this->currentVel.x = maxVel * this->dir.x;
	this->currentVel.y = maxVel * this->dir.y;
	this->maxRotation = maxRotation;

	this->lifeTimeMax = lifeTime;
	this->lifeTime = lifeTime;
}

Particle::~Particle()
{

}

void Particle::update(const float &dt)
{
	//Lifetime
	double lifeTimePercent = 0.f;
	if (this->lifeTime > 0.f)
	{
		this->lifeTime -= 1.f * dt * this->dtMultiplier;

		lifeTimePercent = static_cast<double>(this->lifeTime / this->lifeTimeMax);

		//Reduce alpha
		if(this->sprite.getColor().a > 0)	
		{
			this->sprite.setColor(Color(
				this->color.r, 
				this->color.g,
				this->color.b,
				this->sprite.getColor().a - 1 * dt * this->dtMultiplier)
			);

			if (this->sprite.getColor().a > 255)
				this->sprite.setColor(Color(
					this->color.r,
					this->color.g,
					this->color.b,
					0)
				);
		}

		//Movement
		this->currentVel.x += this->deceleration * -this->dir.x * dt * this->dtMultiplier;
		this->currentVel.y += this->deceleration * -this->dir.y * dt * this->dtMultiplier;

		this->sprite.move(
			this->currentVel.x * dt * this->dtMultiplier,
			this->currentVel.y * dt * this->dtMultiplier
			);

		this->sprite.rotate(this->maxRotation * dt * this->dtMultiplier);
	}
}

void Particle::draw(RenderTarget &target)
{
	target.draw(this->sprite);
}
