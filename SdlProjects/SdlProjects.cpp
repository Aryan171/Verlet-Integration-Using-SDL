#include "header.h"
#include "Ball.h"

int main(int argc, char* argv[])
{
	SDL_InitSubSystem(SDL_INIT_EVERYTHING);

	const int numberOfBalls = 10;
	const int steps = 10;

	int mouseX = 0, mouseY = 0;

	SDL_DisplayMode dm;
	if (SDL_GetCurrentDisplayMode(0, &dm) != 0) {
		std::cout << "error while determining the display resolution\n" <<
			SDL_GetError() << std::endl;
		return 1;
	}

	screenRect.x = dm.w;
	screenRect.y = dm.h;

	screenMid = screenRect / 2;

	SDL_Window* window = SDL_CreateWindow("my game", 0, 0,
		screenRect.x, screenRect.y, SDL_WINDOW_FULLSCREEN | SDL_WINDOW_BORDERLESS);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_ShowCursor(SDL_DISABLE);
	SDL_Event ev;

	SDL_Rect dRect;
	const SDL_Rect* dRectPtr = &dRect;

	bool running = true;

	// making it so that when the renderer draws it blends the color according to the alpha value
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	Ball balls[numberOfBalls];

	Vec2d gravity = Vec2d(0.0f, 1000.0f);

	for (Ball& b : balls) {
		float radius = round(RandFloat(screenRect.y / 40, screenRect.y / 20));
		Color c = Color(RandInt(256), RandInt(256), RandInt(256), 255);
		Vec2d position = Vec2d(RandFloat(screenRect.x), RandFloat(screenRect.y)),
			velocity = Vec2d(RandFloat(-screenRect.y / 20, screenRect.y / 20), RandFloat(-screenRect.y / 20));

		auto texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 2 * radius, 2 * radius);

		// making it so that the color of the texture when copied to the window is blended according to the alpha value
		SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

		SDL_SetRenderTarget(renderer, texture);
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, c.red, c.green, c.blue, c.alpha);
		FillCircle(renderer, radius, radius, radius);

		b = Ball(position, position, radius, c, texture);
	}

	std::chrono::steady_clock::time_point start, end;
	float deltaTimeSec = 0.1f;

	while (running) {
		start = std::chrono::steady_clock::now();

		// taking input from user
		while (SDL_PollEvent(&ev)) {
			if (ev.type == SDL_QUIT) {
				running = false;
			}
			else if (ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_ESCAPE) {
				running = false;
			}
			else if (ev.type == SDL_MOUSEMOTION) {
				SDL_GetMouseState(&mouseX, &mouseY);
			}
		}
		balls[0].positionCurrent.x = mouseX;
		balls[0].positionCurrent.y = mouseY;

		// updating the scene
		for (int i = 1; i < numberOfBalls; i++) {
			AccelerateBall(gravity, balls[i], ACCELERATE);
			ApplyConstraint(balls[i], CIRCLE_CONSTRAINT);
			UpdateBall(balls[i], deltaTimeSec);
		}
		for (int i = 0; i < steps; ++i) {
				ApplyCollision(balls, numberOfBalls, 1.0f / (float)steps);
		}

		// drawing the scene
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_SetRenderTarget(renderer, nullptr); 
		// setting the target to nullptr sets the target to the window
		SDL_RenderClear(renderer);

		for (Ball& b : balls) {
			dRect.x = b.positionCurrent.x - b.radius;
			dRect.y = b.positionCurrent.y - b.radius;
			dRect.w = b.radius * 2;
			dRect.h = dRect.w;
			SDL_SetRenderDrawColor(renderer, b.color.red, b.color.green, b.color.blue, b.color.alpha);
			SDL_RenderCopy(renderer, b.texture, NULL, dRectPtr);
		}

		// presenting the scene
		SDL_RenderPresent(renderer);
		end = std::chrono::steady_clock::now();
		deltaTimeSec = (float)(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()) / 1000000.0;
		
		//********************
		// the FPS printed by this function is the fps that 
		// will be achieved when this print function is removed
		//std::cout << "fps = " << 1 / deltaTimeSec << std::endl;
		//********************
	}

	return 0;
}

void AccelerateBall(const Vec2d& a, Ball& b, int accelerationType) {
	switch (accelerationType) {
	case ACCELERATE:
		b.acceleration += a;
		break;
	}
}

void ApplyConstraint(Ball& b, int constraintType) {
	switch (constraintType) {
	case CIRCLE_CONSTRAINT:
		if(Distance(b.positionCurrent, screenMid) >= screenMid.y * 0.75 - b.radius) {
				b.positionCurrent = (Normalize(b.positionCurrent - screenMid) * (screenMid.y * 0.75 - b.radius)) + screenMid;
			}
		break;

	case SCREEN_CONSTRAINT:
		if (b.positionCurrent.x > screenRect.x - b.radius) {
			b.positionCurrent.x = screenRect.x - b.radius;
		}
		else if (b.positionCurrent.x < b.radius) {
			b.positionCurrent.x = b.radius;
		}

		if (b.positionCurrent.y > screenRect.y - b.radius) {
			b.positionCurrent.y = screenRect.y - b.radius;
		}
		else if (b.positionCurrent.y < b.radius) {
			b.positionCurrent.y = b.radius;
		}

		break;
	}
}

// handles the collision of the balls
// bIndex is the index of the passed ball in the array of Balls
// make shure that if ApplyCollision is called multiple times per frame then
// the sum of all the stepLengths passed to the function is = 1
// if the function is called only once per frame then put stepLength = 1
void ApplyCollision(Ball balls[], int len, float stepLength) {
	Vec2d collisionAxis;
	float distance;
	float finalVelocity;
	for (int i = 0; i < len; ++i) {
		for (int j = i + 1; j < len; ++j) {
			distance = Distance(balls[i].positionCurrent, balls[j].positionCurrent);
			if (distance < balls[i].radius + balls[j].radius) {
				collisionAxis = Normalize(balls[i].positionCurrent - balls[j].positionCurrent);
				finalVelocity = (1 - distance / (balls[i].radius + balls[j].radius));
				balls[i].positionCurrent += collisionAxis * finalVelocity * balls[j].radius * stepLength;
				balls[j].positionCurrent -= collisionAxis * finalVelocity * balls[i].radius * stepLength;
			}
		}
	}
}

void DrawCircle(SDL_Renderer* renderer, int32_t centreX, int32_t centreY, int32_t radius)
{
	const int32_t diameter = (radius * 2);

	int32_t x = (radius - 1);
	int32_t y = 0;
	int32_t tx = 1;
	int32_t ty = 1;
	int32_t error = (tx - diameter);

	while (x >= y)
	{
		//  Each of the following renders an octant of the circle
		SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
		SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
		SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
		SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
		SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
		SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
		SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
		SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

		if (error <= 0)
		{
			++y;
			error += ty;
			ty += 2;
		}

		if (error > 0)
		{
			--x;
			tx += 2;
			error += (tx - diameter);
		}
	}
}

void FillCircle(SDL_Renderer* renderer, int32_t centerX, int32_t centerY, int32_t radius) {
	int32_t bx = centerX + radius, by = centerY + radius, y;
	double radiusSqr = radius * radius;

	for (int32_t x = centerX - radius; x <= bx; x++) {
		y = sqrt(radiusSqr - pow((double)(x - centerX), 2));
		SDL_RenderDrawLine(renderer, x, centerY - y, x, centerY + y);
	}
}