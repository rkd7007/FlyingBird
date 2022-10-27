#include "MyAnimationSprite.h"
#include <stdio.h>

CMyAnimationSprite::CMyAnimationSprite()
{
	m_nFrame = 0;
	m_interval = 0.0f;
	m_aniType = 0;

	isUp = false;

	m_xPos = 60.0f;
	m_yPos = 180.0f;
}

CMyAnimationSprite::~CMyAnimationSprite()
{
}

void CMyAnimationSprite::Create(LPDIRECT3DDEVICE9 pDevice, char *filename)
{
	FILE *fp = fopen(filename, "rt");
	if (fp == NULL)
		return;

	char imgname[128];

	fscanf(fp, "%s", imgname);
	CMySprite::Create(pDevice, imgname);

	fscanf(fp, "%d", &m_maxAni);

	for (int ani = 0; ani < m_maxAni; ani++)
	{
		int aniIndex, nFrame;
		fscanf(fp, "%d %d", &aniIndex, &nFrame);

		m_infoFrame[aniIndex] = nFrame;

		int left, top, width, height;
		float duration;

		for (int i = 0; i < nFrame; i++)
		{
			fscanf(fp, "%d %d %d %d %f", &left, &top, &width, &height, &duration);
			SetRect(&m_rcFrame[aniIndex][i], left, top, width, height);
			m_fDuration[aniIndex][i] = duration;
		}
	}

	fclose(fp);
}

void CMyAnimationSprite::OnDraw()
{
	RECT srcRect = m_rcFrame[m_aniType][m_nFrame];
	srcRect.bottom = srcRect.top + m_rcFrame[m_aniType][m_nFrame].bottom;
	srcRect.right = srcRect.left + m_rcFrame[m_aniType][m_nFrame].right;

	D3DXVECTOR3 vCenter(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vPosition(m_xPos, m_yPos, 0.0f);

	m_rcCollision.left = (int)m_xPos;
	m_rcCollision.top = (int)m_yPos;
	m_rcCollision.right = m_rcCollision.left + m_rcFrame[m_aniType][m_nFrame].right;
	m_rcCollision.bottom = m_rcCollision.top + m_rcFrame[m_aniType][m_nFrame].bottom;

	m_pDonutSprite[0]->Begin(D3DXSPRITE_ALPHABLEND);

	m_pDonutSprite[0]->Draw(m_pDonutTexture[0],
		&srcRect,
		&vCenter,
		&vPosition,
		D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f));

	m_pDonutSprite[0]->End();
}

void CMyAnimationSprite::OnUpdate(float time)
{
	m_interval += time;
	if (m_interval > m_fDuration[m_aniType][m_nFrame])
	{
		m_interval = 0.0f;
		if(!isUp)
			m_yPos += 3; //계속 내려감
		else
		{
			m_yPos -= 10;
			isUp = false;
		}
		m_nFrame++;
		if (m_nFrame >= m_infoFrame[m_aniType]) m_nFrame = 0;
	}

	OnDraw();
}

BOOL CMyAnimationSprite::isTrigger(RECT rect)
{
	if (m_rcCollision.left < rect.right &&
		m_rcCollision.top < rect.bottom &&
		m_rcCollision.right > rect.left &&
		m_rcCollision.bottom > rect.top)
		return true;
	return false;
}

BOOL CMyAnimationSprite::isObstacleTrigger()
{
	if (m_yPos <= 30 || m_yPos >= 310)
		return true;
	return false;
}