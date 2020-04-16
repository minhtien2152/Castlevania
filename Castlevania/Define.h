#pragma once

#define WINDOW_CLASS_NAME		L"Castlevania"
#define MAIN_WINDOW_TITLE		L"Castlevania"


#define SCREEN_WIDTH 512
#define SCREEN_HEIGHT 480

#define MAX_FRAME_RATE 90

#define BACKGROUND_COLOR D3DCOLOR_XRGB(0, 0, 0)


#define ID_TEX_BBOX -100

// Simon
#define SIMON_BBOX_WIDTH 30
#define SIMON_BBOX_HEIGHT 63
#define SIMON_JUMP_BBOX_HEIGHT 43
#define SIMON_PULL_UP 11
#define SIMON_DEFAULT_HEALTH 16

// Ground
#define GROUND_BBOX_WIDTH			32
#define GROUND_BBOX_HEIGHT			32

//Whip
#define NORMAL_WHIP_BBOX_WIDTH		55
#define LONG_CHAIN_BBOX_WIDTH		85
#define WHIP_BBOX_HEIGHT			15


//Candle
#define BIG_CANDLE_BBOX_WIDTH			32
#define BIG_CANDLE_BBOX_HEIGHT			64
#define SMALL_CANDLE_BBOX_WIDTH		16
#define SMALL_CANDLE_BBOX_HEIGHT	32
enum Object_Type
{
	PORTAL,
	SIMON,
	GROUND,
	ZOMBIE,
	CANDLE,
	BAT,
	WHIP
};


#define ITEM_APPEAR_TIME 3000
#define GRAVITY 0.0001f
enum Item_Type
{
	STOPWATCH_ITEM,
	SMALLHEART,
	LARGEHEART,
	REDMONEYBAG,
	WHITEMONEYBAG,
	BLUEMONEYBAG,
	CHAIN,
	DAGGER_ITEM,
	CROSS,
	AXE_ITEM,
	INVISPOTION,
	BOOMERANG_ITEM,
	HOLYWATER_ITEM,
	PORKCHOP,
	DOUBLESHOT,
	TRIPLESHOT,
	MAGICCRYSTAL

};

enum Weapon_Type
{
	STOPWATCH,
	DAGGER,
	AXE,
	HOLYWATER,
	BOOMERANG
};

enum Effect_Type
{
	SPARK_EFFECT,
	FIRE_EFFECT,
	BROKEN_BRICK_EFFECT
};

#define DEFAULT_TIME_PLAY		300

#define SIMON_HEALTH 0
#define MISSING_HEALTH 1
#define ENEMY_HEALTH 2

#define HEALTH_FRAME_WIDTH 8



#define HOLY_WATER_BBOX_WIDTH	32
#define HOLY_WATER_BBOX_HEIGHT	26
#define HOLY_WATER_SPEED_X	0.25f
#define HOLY_WATER_SPEED_Y	0.1f

#define DAGGER_BBOX_WIDTH		34
#define DAGGER_BBOX_HEIGHT		18
#define DAGGER_SPEED		0.3f

#define AXE_BBOX_WIDTH			30
#define AXE_BBOX_HEIGHT			28
#define AXE_SPEED_X				0.2f
#define AXE_SPEED_Y				0.45f

#define BOOMERANG_BBOX_WIDTH	28
#define BOOMERANG_BBOX_HEIGHT	28
#define	BOOMERANG_SPEED		0.5f
#define BOOMERANG_TURNBACK_SPEED	0.01f

#define FONT_FILEPATH	L"Resources\\Font\\prstart.ttf"
#define TILE_WIDTH 32
#define TILE_HEIGHT 32

#define STATUS_BOARD_HEIGHT 80

#define DEFAULT_CAM_X 0
#define default_CAM_Y 0

#define GAME_STATE_MENU	0
#define GAME_STATE_INTROWALK	1
#define GAME_STATE_PLAYSCENE	2
