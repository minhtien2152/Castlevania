#include "BrickEffect.h"
#include "Define.h"
#define RANDOM_VELOCITY_RANGE	15
BrickEffect::BrickEffect(float x, float y) : Effect(x,y)
{
	animation_set = CAnimationSets::GetInstance()->Get(Effect_Type::BROKEN_BRICK_EFFECT);
	InitBricks();
}

void BrickEffect::InitBricks()
{
	int num_brick = rand() % 4 + 3;
	for (int i = 0; i < num_brick; i++)
	{
		LPGAMEOBJECT brick = new FallingBrick();
		float vx = (float)(rand() % RANDOM_VELOCITY_RANGE +1) / 100;
		if (rand() % 2 == 0)
			vx *= -1;
		float vy = (float)( rand() % RANDOM_VELOCITY_RANGE + 5) / 100;
		brick->SetPosition(x, y);
		brick->SetSpeed(vx, -vy);
		brick->animation_set = animation_set;
		falling_bricks.push_back(brick);
	}
}

void BrickEffect::Update(DWORD dt)
{
	Effect::Update(dt);
	for (auto brick : falling_bricks)
		brick->Update(dt);

}

bool BrickEffect::IsFinished()
{

	for (auto brick : falling_bricks)
		if (brick->y < screen_height)
			return false;
	return true;
}

void BrickEffect::Render()
{
	for (auto brick : falling_bricks)
		brick->Render();
}
