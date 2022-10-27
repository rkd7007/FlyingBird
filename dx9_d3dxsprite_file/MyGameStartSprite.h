#pragma once

#include "MySprite.h"

class CMyGameStartSprite : public CMySprite
{
protected:
	RECT m_rcFrame;
	unsigned short m_nFrame;
	float m_interval;

public:
	CMyGameStartSprite();
	virtual ~CMyGameStartSprite();

	void Create(LPDIRECT3DDEVICE9 pDevice, char* filename);
	void OnDraw();
	void OnUpdate(float time);
};

