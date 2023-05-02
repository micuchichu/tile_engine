#pragma once
#include "headers.h"
#include "textures.h"
#include "globals.h"

class Player
{
public:
	Player();

	void ApplyForce(Vector2 f, float dt);

	void Update(float dt);

	const Vector2 getPos() const;

	const Vector2 getVel() const;

	const Camera2D getCam() const;

	const int getWidth() const;

	const Texture2D getTexture() const;

	void setPos(Vector2 pos);
	void setX(float x);
	void setY(float y);

	void setVel(Vector2 vel);

	void setCamOffset(Vector2 offset);

	void initTexture();

	void Draw();

	void Draw(int frame);

private:
	Vector2 pos;
	Vector2 vel;
	Camera2D cam;
	Texture2D texture;
};