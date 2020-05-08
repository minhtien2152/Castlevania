#include "Camera.h"
#include "Game.h"
Camera::Camera()
{
	cam_x = cam_y = 0.00f;
}

Camera::~Camera()
{
}

void Camera::Update(Simon* simon)
{
	float simon_x, simon_y;
	simon->GetPosition(simon_x, simon_y);
	if (simon_x >= CGame::GetInstance()->GetScreenWidth() / 2 //ria trai
		&& simon_x + CGame::GetInstance()->GetScreenWidth() / 2 <= mapWidth) //ria phai
		cam_x = simon_x - CGame::GetInstance()->GetScreenWidth() / 2;
	else if (simon_x < CGame::GetInstance()->GetScreenWidth() / 2)
		cam_x = 0;
	else if (simon_x + CGame::GetInstance()->GetScreenWidth() / 2 > mapWidth)
		cam_x = mapWidth - CGame::GetInstance()->GetScreenWidth()+1;
}

void Camera::GetCamPosition(float& x, float& y)
{
	x = cam_x;
	y = cam_y;
}

bool Camera::IsInCam(LPGAMEOBJECT obj)
{
	float obj_x, obj_y;
	obj->GetPosition(obj_x, obj_y);
	
	return obj_x>cam_x && obj_x < cam_x + CGame::GetInstance()->GetScreenWidth();
}

void Camera::SetMapWidth(int width)
{
	mapWidth = width;
}

int Camera::GetMapWidth()
{
	return mapWidth;
}
