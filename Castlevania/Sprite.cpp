#include "Sprite.h"
#include "Game.h"

CSprite::CSprite(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex)
{
	this->id = id;
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
	this->texture = tex;
}


void CSprite::Draw(float x, float y,int nx,int alpha)
{
	CGame* game = CGame::GetInstance();
	if(nx==1)
		game->Draw(x, y, texture, left, top, right, bottom,alpha);
	else
		game->DrawFlipVertical(x, y, texture, left, top, right, bottom,alpha);
}
