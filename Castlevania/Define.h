#pragma once

#define WINDOW_CLASS_NAME		L"Castlevania"
#define MAIN_WINDOW_TITLE		L"Castlevania"


#define SCREEN_WIDTH 528
#define SCREEN_HEIGHT 504

#define GRID_CELL_WIDTH		SCREEN_WIDTH/2
#define GRID_CELL_HEIGHT	SCREEN_HEIGHT/2

#define MAX_FRAME_RATE 90

#define BACKGROUND_COLOR D3DCOLOR_XRGB(0, 0, 0)

#define INTRO_SCENE_ID 0
#define ID_TEX_BBOX -100

// Simon
#define SIMON_BBOX_WIDTH 30
#define SIMON_BBOX_HEIGHT 63
#define SIMON_JUMP_BBOX_HEIGHT 43
#define SIMON_PULL_UP 16

#define SIMON_DEFAULT_HEALTH 16
#define SIMON_DEFAULT_HEART 5
#define SIMON_DEFAULT_LIFE 5


#define PORTAL_BBOX_WIDTH 30
#define PORTAL_BBOX_HEIGHT 63

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
	CANDLE=20,
	TORCH,
	PLATFORM,
	BAT,
	KNIGHT,
	SKELETON,
	ZOMBIE,
	HUNCHBACK,
	GHOST,
	RAVEN,
	BUMPER,
	STAIR_OBJECT
};



#define ITEM_APPEAR_TIME 3000
#define GRAVITY 0.001f
#define	ITEM_ANI_SET_ID 11
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
	CHICKEN,
	DOUBLESHOT,
	TRIPLESHOT,
	MAGICCRYSTAL,
	CROWN,
	CHEST

};

enum Weapon_Type
{
	WHIP=4,
	DAGGER,
	BOOMERANG,
	AXE,
	HOLYWATER,
	STOPWATCH,
	BONE
	
};


enum Effect_Type
{
	SPARK_EFFECT=13,
	FIRE_EFFECT,
	BROKEN_BRICK_EFFECT,
};


#define DEFAULT_TIME_PLAY		300

#define SIMON_HEALTH 0
#define MISSING_HEALTH 1
#define ENEMY_HEALTH 2

#define HEALTH_FRAME_WIDTH 8


#define HOLY_WATER_BBOX_WIDTH	32
#define HOLY_WATER_BBOX_HEIGHT	26
#define HOLY_WATER_SPEED_X	0.25f
#define HOLY_WATER_SPEED_Y	0.3f

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
#define BOOMERANG_TURNBACK_SPEED	0.009f

#define FONT_FILEPATH	L"Resources\\Font\\prstart.ttf"
#define TILE_WIDTH 32
#define TILE_HEIGHT 32

#define STATUS_BOARD_HEIGHT 80

#define DEFAULT_CAM_X 0
#define DEFAULT_CAM_Y 0

#define GAME_STATE_MENU	0
#define GAME_STATE_INTROWALK	1
#define GAME_STATE_PLAYSCENE	2

#define SUB_WEAPON_RECT_TEX_ID 0
#define DEATH_TIMER_DELAY 2000