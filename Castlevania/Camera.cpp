#include "Camera.h"
#include "Game.h"
Camera::Camera()
{
	cam_x = cam_y = 0.00f;
	isInBossFight = false;
}

Camera::~Camera()
{
}

void Camera::Update(Simon* simon)
{
	if (isInBossFight)
		return;
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
	float l,t,r,b;
	obj->GetBoundingBox(l, t, r, b);
	int screen_width = CGame::GetInstance()->GetScreenWidth();
	return l > cam_x &&  r < cam_x + screen_width
		&& t - STATUS_BOARD_HEIGHT> cam_y
		&& b - STATUS_BOARD_HEIGHT < cam_y + mapHeight ;
}

void Camera::SetMapWidth(int width)
{
	mapWidth = width;
}

void Camera::SetMapHeight(int height)
{
	mapHeight = height;
}

int Camera::GetMapWidth()
{
	return mapWidth;
}

void Camera::InBossFight()
{
	isInBossFight = true;
}
