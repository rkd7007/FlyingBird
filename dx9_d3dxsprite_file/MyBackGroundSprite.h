#pragma once

#include "MySprite.h"

class CMyBackGroundSprite : public CMySprite
{
protected:
	RECT m_rcFrame;
	unsigned short m_nFrame;
	float m_interval;

	float m_xPos[2];
	float m_yPos;

public:
	CMyBackGroundSprite();
	virtual ~CMyBackGroundSprite();

	void Create(LPDIRECT3DDEVICE9 pDevice, char* filename);
	void OnDraw();
	void OnUpdate(float time);
};

