#pragma once
#include "MySprite.h"

#define MAX_ANI					8
#define MAX_ANI_FRAME     8

class CMyAnimationSprite : public CMySprite
{
protected:
	RECT m_rcFrame[MAX_ANI][MAX_ANI_FRAME];
	unsigned char  m_maxAni;
	unsigned short m_nFrame;
	float m_interval;

	int m_infoFrame[MAX_ANI];
	unsigned char m_aniType;

	float m_fDuration[MAX_ANI][MAX_ANI_FRAME];

	//캐릭터 콜라이더
	RECT  m_rcCollision;

public:
	//캐릭터 위치
	float m_xPos;
	float m_yPos;

	bool isUp;

	CMyAnimationSprite();
	virtual ~CMyAnimationSprite();

	void Create(LPDIRECT3DDEVICE9 pDevice, char *filename);
	void OnDraw();
	void OnUpdate(float time);

	BOOL isTrigger(RECT rect);
	BOOL isObstacleTrigger();
};

