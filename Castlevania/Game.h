#pragma once
#include<Windows.h>
#include<unordered_map>
#include<d3d9.h>
#include<d3dx9.h>

#include"Utils.h"
#include"Scene.h"
#include "Camera.h"
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#define KEYBOARD_BUFFER_SIZE 1024
using namespace std;

typedef CKeyEventHandler * LPKEYEVENTHANDLER;
class CGame
{
	static CGame* __instance;
	HWND hWnd;									// Window handle

	LPDIRECT3D9 d3d = NULL;						// Direct3D handle
	LPDIRECT3DDEVICE9 d3ddv = NULL;				// Direct3D device object

	LPDIRECT3DSURFACE9 backBuffer = NULL;
	LPD3DXSPRITE spriteHandler = NULL;			// Sprite helper library to help us draw 2D image on the screen 

	LPDIRECTINPUT8       di;		// The DirectInput object         
	LPDIRECTINPUTDEVICE8 didv;		// The keyboard device 

	BYTE  keyStates[256];			// DirectInput keyboard state buffer 
	DIDEVICEOBJECTDATA keyEvents[KEYBOARD_BUFFER_SIZE];		// Buffered keyboard data

	ID3DXFont* font;

	LPKEYEVENTHANDLER keyHandler;

	Camera* camera;
	int screen_width;
	int screen_height;

	int state;

	unordered_map<int, LPSCENE> scenes;
	int current_scene;

	void _ParseSection_SETTINGS(string line);
	void _ParseSection_SCENES(string line);
public:
	void Init(HWND hWnd);
	void Draw(float x, float y, int nx, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom,int alpha=255 , bool accordingCam = 1 );
	

	void InitKeyboard();
	void SetKeyHandler(LPKEYEVENTHANDLER handler) { keyHandler = handler; }
	int IsKeyDown(int KeyCode);
	void ProcessKeyboard();

	void Load(LPCWSTR gameFile);
	LPSCENE GetCurrentScene() { return scenes[current_scene]; }
	void SwitchScene(int scene_id);

	int GetScreenWidth() { return screen_width; }
	int GetScreenHeight() { return screen_height; }

	static void SweptAABB(
		float ml,			// move left 
		float mt,			// move top
		float mr,			// move right 
		float mb,			// move bottom
		float dx,			// 
		float dy,			// 
		float sl,			// static left
		float st,
		float sr,
		float sb,
		float& t,
		float& nx,
		float& ny);

	static bool IsOverlapping(
		float ml,			// move left 
		float mt,
		float mr,
		float mb,
		float sl,			// static left
		float st,
		float sr,
		float sb);


	LPDIRECT3DDEVICE9 GetDirect3DDevice() { return this->d3ddv; }
	LPDIRECT3DSURFACE9 GetBackBuffer() { return this->backBuffer; }
	LPD3DXSPRITE GetSpriteHandler() { return this->spriteHandler; }

	void SetCamera(Camera* camera);

	ID3DXFont* GetFont();

	void SetState(int state);
	int GetState();
	static CGame* GetInstance();
	~CGame();
};

