#pragma once

#include "MySprite.h"

class CMyObstacleSprite : public CMySprite
{
protected:
	RECT m_rcFrame;
	unsigned short m_nFrame;
	float m_interval;

	int speed; //��ֹ� �̵��ӵ�
	
    //��ֹ� ��ġ
	float m_xPos;
	float m_yPos;

public:
	//��ֹ� �ݶ��̴�
	RECT  m_rcCollision;

	CMyObstacleSprite();
	virtual ~CMyObstacleSprite();

	void Create(LPDIRECT3DDEVICE9 pDevice, char* filename);
	void OnDraw();
	void OnUpdate(float time);
};

