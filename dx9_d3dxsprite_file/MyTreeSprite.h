#pragma once

#include "MySprite.h"

class CMyTreeSprite : public CMySprite
{
protected:
	RECT m_rcFrame;
	unsigned short m_nFrame;
	float m_interval;

	int speed; //장애물 이동속도

	//장애물 위치
	float m_xPos;
	float m_yPos;

public:
	//장애물 콜라이더
	RECT  m_rcCollision;

	CMyTreeSprite();
	virtual ~CMyTreeSprite();

	void Create(LPDIRECT3DDEVICE9 pDevice, char* filename);
	void OnDraw();
	void OnUpdate(float time);
};

