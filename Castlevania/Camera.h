#pragma once
#include "Define.h"
#include"Simon.h"

class Camera
{
	float cam_x, cam_y;
public:
	Camera();
	~Camera();
	void Update(Simon * simon);
	void GetCamPosition(float& x, float& y);
	bool IsInCam(LPGAMEOBJECT obj);
};
