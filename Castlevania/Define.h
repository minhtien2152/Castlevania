#pragma once

#define WINDOW_CLASS_NAME		L"Castlevania"
#define MAIN_WINDOW_TITLE		L"Castlevania"


#define SCREEN_WIDTH 512
#define SCREEN_HEIGHT 480

#define MAX_FRAME_RATE 90

#define BACKGROUND_COLOR D3DCOLOR_XRGB(255, 255, 255)


#define ID_TEX_BBOX -100

// Simon
#define SIMON_BBOX_WIDTH 30
#define SIMON_BBOX_HEIGHT 63
#define SIMON_JUMP_BBOX_HEIGHT 43
#define SIMON_PULL_UP 11

// Ground
#define GROUND_BBOX_WIDTH			32
#define GROUND_BBOX_HEIGHT			32

enum Object_Type
{
	SIMON	=1,
	GROUND,
	ZOMBIE,
	CANDLE,
	BAT
};