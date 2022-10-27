#include "MyGameStartSprite.h"
#include <stdio.h>

CMyGameStartSprite::CMyGameStartSprite()
{
	m_nFrame = 0;
	m_interval = 0.0f;
}

CMyGameStartSprite::~CMyGameStartSprite()
{
}

void CMyGameStartSprite::Create(LPDIRECT3DDEVICE9 pDevice, char* filename)
{
	CMySprite::Create(pDevice, filename);
	SetRect(&m_rcFrame, 0, 0, 640, 480);
}

void CMyGameStartSprite::OnDraw()
{
	RECT srcRect = m_rcFrame;
	srcRect.bottom = srcRect.top + m_rcFrame.bottom;
	srcRect.right = srcRect.left + m_rcFrame.right;

	D3DXVECTOR3 vCenter(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vPosition(0.0f, 0.0f, 0.0f);

	m_pDonutSprite[1]->Begin(D3DXSPRITE_ALPHABLEND);

	m_pDonutSprite[1]->Draw(m_pDonutTexture[1],
		&srcRect,
		&vCenter,
		&vPosition,
		D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f));

	m_pDonutSprite[1]->End();
}

void CMyGameStartSprite::OnUpdate(float time)
{
	OnDraw();
}