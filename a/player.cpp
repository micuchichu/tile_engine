#include "player.h"
#include "utils.h"

Player::Player()
{
	pos = { 0, 0 };
	vel = { 0, 0 };
	cam.offset = { 0, 0 };
	cam.rotation = 0;
	cam.target = { 0, 0 };
	cam.zoom = 1;
}

void Player::ApplyForce(Vector2 f, float dt)
{
	vel.x += f.x * dt;
	vel.y += f.y * dt;
}

void Player::Update(float dt)
{
	pos.x += vel.x * dt;
	pos.y += vel.y * dt;
	cam.target = pos;
}

const Vector2 Player::getPos() const { return pos; }

const Vector2 Player::getVel() const { return vel; }

const Camera2D Player::getCam() const { return cam; }

const int Player::getWidth() const { return 64; }

const Texture2D Player::getTexture() const { return this->texture_walk; }

void Player::setPos(Vector2 pos) { this->pos = pos; }
void Player::setX(float x) { this->pos.x = x; }
void Player::setY(float y) { this->pos.y = y; }

void Player::setVel(Vector2 vel) { this->vel = vel; }

void Player::setCamOffset(Vector2 offset) { this->cam.offset = offset; }

void Player::initTexture() 
{ 
	texture_walk = LoadTexture("./textures/player/mogu_walk.png");
	texture_idle = LoadTexture("./textures/player/mogu_idle.png");
	texture_jump = LoadTexture("./textures/player/mogu_jump.png");
}

void Player::Draw()
{
	DrawTextureV(texture_idle, pos, RAYWHITE);
}

void Player::Draw(int frame)
{
	if (vel.y < 0)
	{
		frame = pos.y / 64;
		if (sgn(vel.y) - 1)
			frame = texture_jump.width - frame;
		Rectangle frameR = { 64 * frame, 0, 64 * sgn(vel.x), texture_jump.height };

		DrawTextureRec(texture_jump, frameR, pos, RAYWHITE);
	}
	else if (abs(vel.x) > 10)
	{
		if (sgn(vel.x) - 1)
			frame = texture_walk.width - frame;
		Rectangle frameR = { 64 * frame, 0, 64 * sgn(vel.x), texture_walk.height };

		DrawTextureRec(texture_walk, frameR, pos, RAYWHITE);
	}
	else
		DrawTextureV(texture_idle, pos, RAYWHITE);
}