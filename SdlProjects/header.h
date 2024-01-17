#pragma once
#include <random>
#include <SDL.h>
#include <iostream>
#include "__msvc_chrono.hpp"
#include "Ball.h"

std::random_device rd;
std::uniform_real_distribution<float> dist(0.0f, 1.0f);

Vec2d screenRect; // a 2d vector having the height and width info of the screen

Vec2d screenMid; // a 2d vector pointing to the middle of the screen

const int CIRCLE_CONSTRAINT = 0,
SCREEN_CONSTRAINT = 1;

const int ACCELERATE = 0;

float RandFloat(float maxVal) {
	return dist(rd) * maxVal;
}

int RandInt(int maxVal) {
	return (int)(maxVal * dist(rd));
}

float RandFloat(float minVal, float maxVal) {
	return RandFloat(maxVal - minVal) + minVal;
}

void DrawCircle(SDL_Renderer*, int32_t, int32_t, int32_t);

void FillCircle(SDL_Renderer*, int32_t, int32_t, int32_t);

void ApplyConstraint(Ball&, const int);

void ApplyCollision(Ball[], int, float);

void AccelerateBall(const Vec2d&, Ball&, int);