#pragma once
#include"AnimationFrame.h"

class CAnimation
{
	DWORD aniStartTime;
	DWORD lastFrameTime;
	int defaultTime;
	int currentFrame;
	DWORD totalFrameTime;
	vector<LPANIMATION_FRAME> frames;
public:
	CAnimation(int defaultTime = 100);
	void SetAniStartTime(DWORD t) { aniStartTime = t; }
	bool IsOver() { return GetTickCount() - aniStartTime >= totalFrameTime; }
	bool CheckAniTime(DWORD t){ return GetTickCount() - aniStartTime >= t; }
	void Add(int spriteId, DWORD time = 0);
	void Render(float x, float y, int nx = 1, bool isUpdatingFrame = 1, bool accordingCam=1, int alpha = 255);

	void SetFrame(int x) { currentFrame = x; }
	void Reset() { currentFrame = -1; }
	int GetCurrentFrame() { return currentFrame; }
	bool IsRenderingLastFrame();
	bool IsRenderingFirstFrame();
	LPANIMATION_FRAME GetLPFirstFrame();
};

typedef CAnimation* LPANIMATION;
