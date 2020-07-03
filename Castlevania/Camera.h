#pragma once
#include "Define.h"
#include"Simon.h"

class Camera
{
	float cam_x, cam_y;
	int mapWidth;
	int mapHeight;
	bool isInBossFight;
public:
	Camera();
	~Camera();
	void Update(Simon * simon);
	void GetCamPosition(float& x, float& y);
	bool IsInCam(LPGAMEOBJECT obj);
	void SetMapWidth(int width);
	void SetMapHeight(int height);
	int GetMapWidth();
	void InBossFight();
};

