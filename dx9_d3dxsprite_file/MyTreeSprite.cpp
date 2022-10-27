#include "MyTreeSprite.h"
#include <stdio.h>
#include <time.h>

CMyTreeSprite::CMyTreeSprite()
{
	m_nFrame = 0;
	m_interval = 0.0f;

	speed = 10;

	m_xPos = 620.0f;
	m_yPos = 280.0f;

	srand(time(NULL));
}

CMyTreeSprite::~CMyTreeSprite()
{
}

void CMyTreeSprite::Create(LPDIRECT3DDEVICE9 pDevice, char* filename)
{
	CMySprite::Create(pDevice, filename);
	SetRect(&m_rcFrame, 78, 45, 97, 164);
}

void CMyTreeSprite::OnDraw()
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

	m_pDonutSprite[2]->Begin(D3DXSPRITE_ALPHABLEND);

	m_pDonutSprite[2]->Draw(m_pDonutTexture[2],
		&srcRect,
		&vCenter,
		&vPosition,
		D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f));

	m_pDonutSprite[2]->End();
}

void CMyTreeSprite::OnUpdate(float time)
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