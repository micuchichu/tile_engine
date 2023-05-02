#include "player.h"

float sgn(float n)
{
	if (n < 0)
		return -1;
	else
		return 1;
}

Vector2 norm(Vector2 a)
{
	float length = std::sqrt(a.x * a.x + a.y * a.y);
	if (abs(length) < 1.0f)
	{
		a.x /= length;
		a.y /= length;
	}
	else
		return { sgn(a.x), sgn(a.y)};
	return a;
}

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

const Texture2D Player::getTexture() const { return this->texture; }

void Player::setPos(Vector2 pos) { this->pos = pos; }
void Player::setX(float x) { this->pos.x = x; }
void Player::setY(float y) { this->pos.y = y; }

void Player::setVel(Vector2 vel) { this->vel = vel; }

void Player::setCamOffset(Vector2 offset) { this->cam.offset = offset; }

void Player::initTexture() 
{ 
	texture = LoadTexture("./textures/player/amogu.png"); 
}

void Player::Draw()
{
	DrawTextureV(texture, pos, RAYWHITE);
}

void Player::Draw(int frame)
{
	if(sgn(vel.x) - 1)
		frame = texture.width - frame;
	Rectangle frameR = { 64 * frame, 0, 64 * sgn(vel.x), texture.height};

	DrawTextureRec(texture, frameR, pos, RAYWHITE);
}