#include "MyBackGroundSprite.h"
#include <stdio.h>

CMyBackGroundSprite::CMyBackGroundSprite()
{
	m_nFrame = 0;
	m_interval = 0.0f;

	m_xPos[0] = 0;
	m_xPos[1] = 640;
	m_yPos = 0;
}

CMyBackGroundSprite::~CMyBackGroundSprite()
{
}

void CMyBackGroundSprite::Create(LPDIRECT3DDEVICE9 pDevice, char* filename)
{
	CMySprite::Create(pDevice, filename);
	SetRect(&m_rcFrame, 0, 0, 640, 480);
}

void CMyBackGroundSprite::OnDraw()
{
	RECT srcRect = m_rcFrame;
	srcRect.bottom = srcRect.top + m_rcFrame.bottom;
	srcRect.right = srcRect.left + m_rcFrame.right;

	D3DXVECTOR3 vCenter(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vPosition(m_xPos[0], m_yPos, 0.0f);
	D3DXVECTOR3 vpPosition(m_xPos[1], m_yPos, 0.0f);

	m_pDonutSprite[1]->Begin(D3DXSPRITE_ALPHABLEND);

	m_pDonutSprite[1]->Draw(m_pDonutTexture[1],
		&srcRect,
		&vCenter,
		&vPosition,
		D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f));

	m_pDonutSprite[1]->Draw(m_pDonutTexture[1],
		&srcRect,
		&vCenter,
		&vpPosition,
		D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f));

	m_pDonutSprite[1]->End();
}

void CMyBackGroundSprite::OnUpdate(float time)
{
	m_interval += time;
	if (m_interval > 0.01f)
	{
		m_interval = 0.0f;

		m_xPos[0] -= 2;
		m_xPos[1] -= 2;

		if(m_xPos[0] <= -640)
			m_xPos[0] = 640;
		if (m_xPos[1] <= -640)
			m_xPos[1] = 640;
	}

	OnDraw();
}