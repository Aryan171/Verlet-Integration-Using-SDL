#pragma once
#include <SDL_stdinc.h>
#include "Vec2d.h"
#include <SDL.h>

class Color {
public:
	Uint8 red, green, blue, alpha;

	Color(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha) {
		this->red = red;
		this->green = green;
		this->blue = blue;
		this->alpha = alpha;
	}

	Color() {
		this->red = 0;
		this->green = 0;
		this->blue = 0;
		this->alpha = 0;
	}
};

class Ball {
public:
	Vec2d positionCurrent, positionOld, acceleration;
	float radius;
	Color color;
	SDL_Texture* texture;

	Ball(Vec2d positionCurrent, Vec2d positionOld, float radius, Color color, SDL_Texture* texture) {
		this->positionCurrent = positionCurrent;
		this->positionOld = positionOld;
		this->acceleration = Vec2d();
		this->radius = radius;
		this->color = color;
		this->texture = texture;
	}

	Ball() {
		this->positionCurrent = Vec2d(0.0f, 0.0f);
		this->positionOld = Vec2d(0.0f, 0.0f);
		this->acceleration = Vec2d(0.0f, 0.0f);
		this->radius = 0.0f;
		this->color = Color();
		this->texture = nullptr;
	}
};

void UpdateBall(Ball& b, float deltaTimeSec) {
	Vec2d distMoved = b.positionCurrent - b.positionOld;
	b.positionOld = b.positionCurrent;
	b.positionCurrent += distMoved + b.acceleration * deltaTimeSec * deltaTimeSec;
	b.acceleration.x = 0.0f;
	b.acceleration.y = 0.0f;
}