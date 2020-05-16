#include "Animation.h"
#include "SceneMangager.h"

CAnimation::CAnimation(int defaultTime)
{
	this->defaultTime = defaultTime;
	lastFrameTime = -1;
	currentFrame = -1;
	totalFrameTime = 0;
}

void CAnimation::Add(int spriteId, DWORD time)
{
	if (time == 0) time = this->defaultTime;

	LPSPRITE sprite = CSprites::GetInstance()->Get(spriteId);
	LPANIMATION_FRAME frame = new CAnimationFrame(sprite, time);
	frames.push_back(frame);
	totalFrameTime += time;
}

void CAnimation::Render(float x, float y, int nx, bool isUpdatingFrame, bool accordingCam, int alpha)
{
	DWORD now = GetTickCount();
	if (currentFrame == -1)
	{
		currentFrame = 0;
		lastFrameTime = now;
	}
	if(isUpdatingFrame)
	{
		DWORD t = frames[currentFrame]->GetTime();
		if (now - lastFrameTime > t)
		{
			currentFrame++;
			lastFrameTime = now;
			if (currentFrame == frames.size()) currentFrame = 0;
			//DebugOut(L"now: %d, lastFrameTime: %d, t: %d\n", now, lastFrameTime, t);
		}

	}
	frames[currentFrame]->GetSprite()->Draw(x, y, nx, alpha,accordingCam);
}

bool CAnimation::IsRenderingLastFrame()
{
	return currentFrame == frames.size() - 1;
}

bool CAnimation::IsRenderingFirstFrame()
{
	return currentFrame == 0;
}

LPANIMATION_FRAME CAnimation::GetLPFirstFrame()
{
	return frames[0];
}
