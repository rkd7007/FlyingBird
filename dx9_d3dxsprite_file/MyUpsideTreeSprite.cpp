#include "MyUpsideTreeSprite.h"
#include <stdio.h>
#include <time.h>

CMyUpsideTreeSprite::CMyUpsideTreeSprite()
{
	m_nFrame = 0;
	m_interval = 0.0f;

	speed = 8;

	m_xPos = 620.0f;
	m_yPos = -10.0f;

	srand(time(NULL));
}

CMyUpsideTreeSprite::~CMyUpsideTreeSprite()
{
}

void CMyUpsideTreeSprite::Create(LPDIRECT3DDEVICE9 pDevice, char* filename)
{
	CMySprite::Create(pDevice, filename);
	SetRect(&m_rcFrame, 88, 380, 90, 169);
}

void CMyUpsideTreeSprite::OnDraw()
{
	RECT srcRect = m_rcFrame;
	srcRect.bottom = srcRect.top + m_rcFrame.bottom;
	srcRect.right = srcRect.left + m_rcFrame.right;

	D3DXVECTOR3 vCenter(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vPosition(m_xPos, m_yPos, 0.0f);

	m_rcCollision.left = (int)m_xPos;
	m_rcCollision.top = (int)m_yPos;
	m_rcCollision.right = m_rcCollision.left + m_rcFrame.right;
	m_rcCollision.bottom = m_rcCollision.top + m_rcFrame.bottom;

	m_pDonutSprite[3]->Begin(D3DXSPRITE_ALPHABLEND);

	m_pDonutSprite[3]->Draw(m_pDonutTexture[3],
		&srcRect,
		&vCenter,
		&vPosition,
		D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f));

	m_pDonutSprite[3]->End();
}

void CMyUpsideTreeSprite::OnUpdate(float time)
{
	m_interval += time;
	if (m_interval > 0.1)
	{
		m_interval = 0.0f;
		m_xPos -= speed;
		if (m_xPos <= -100) //화면 밖으로 나가면
		{
			m_xPos = 640.0f;
			speed = rand() % 9 + 5; //5 ~ 13
		}
	}

	OnDraw();
}