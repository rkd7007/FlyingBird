//-----------------------------------------------------------------------------
//           Name: dx9_d3dxsprite.cpp
//         Author: Kevin Harris
//  Last Modified: 02/01/05
//    Description: This sample demonstrates how to create a animated 2D sprite 
//                 using D3DXSprite which is hardware accelerated and fully 
//                 compatible with 3D generated content.
//-----------------------------------------------------------------------------

#define STRICT
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <mmsystem.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "resource.h"
#include "MyAnimationSprite.h"
#include "MyBackGroundSprite.h"
#include "MyTreeSprite.h"
#include "MyUpsideTreeSprite.h"
#include "MyGameOverSprite.h"
#include "MyGameStartSprite.h"

#pragma comment(lib, "winmm.lib")
#include "Mmsystem.h"
#include "Digitalv.h"

//-----------------------------------------------------------------------------
// GLOBALS
//-----------------------------------------------------------------------------
HWND                    g_hWnd          = NULL;
LPDIRECT3D9             g_pD3D          = NULL;
LPDIRECT3DDEVICE9       g_pd3dDevice    = NULL;

float  g_fElpasedTime;
double g_dCurTime;
double g_dLastTime;
float  g_fSpinX = 0.0f;
float  g_fSpinY = 0.0f;
int g_stage = 0; //0 = 메뉴화면, 1 = 게임 중, 2 = 게임오버

CMyAnimationSprite g_aniSprite; //캐릭터
CMyBackGroundSprite g_backGround; //배경
CMyTreeSprite g_tree; //나무 아래
CMyUpsideTreeSprite g_upsideTree; //나무 위
CMyGameOverSprite g_gameOver; //게임 종료
CMyGameStartSprite g_gameStart; //메뉴화면

//사운드
MCI_OPEN_PARMS m_mciOpenParms;
MCI_PLAY_PARMS m_mciPlayParms;
DWORD m_dwDeviceID;
MCI_OPEN_PARMS mciOpen;
MCI_PLAY_PARMS mciPlay;

int dwID;

//-----------------------------------------------------------------------------
// PROTOTYPES
//-----------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
				   LPSTR lpCmdLine, int nCmdShow);
LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void init(void);
void shutDown(void);
void render(void);

//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------
int WINAPI WinMain(	HINSTANCE hInstance,
					HINSTANCE hPrevInstance,
					LPSTR     lpCmdLine,
					int       nCmdShow )
{
	WNDCLASSEX winClass; 
	MSG        uMsg;

    memset(&uMsg,0,sizeof(uMsg));
    
	winClass.lpszClassName = "MY_WINDOWS_CLASS";
	winClass.cbSize        = sizeof(WNDCLASSEX);
	winClass.style         = CS_HREDRAW | CS_VREDRAW;
	winClass.lpfnWndProc   = WindowProc;
	winClass.hInstance     = hInstance;
	winClass.hIcon	       = LoadIcon(hInstance, (LPCTSTR)IDI_DIRECTX_ICON);
    winClass.hIconSm	   = LoadIcon(hInstance, (LPCTSTR)IDI_DIRECTX_ICON);
	winClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
	winClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	winClass.lpszMenuName  = NULL;
	winClass.cbClsExtra    = 0;
	winClass.cbWndExtra    = 0;

	if( !RegisterClassEx(&winClass) )
		return E_FAIL;

	g_hWnd = CreateWindowEx( NULL, "MY_WINDOWS_CLASS", 
		                     "Direct3D (DX9) - Creating 2D Sprites with D3DXSPRITE",
						     WS_OVERLAPPEDWINDOW | WS_VISIBLE,
					         0, 0, 640, 480, NULL, NULL, hInstance, NULL );

	if( g_hWnd == NULL )
		return E_FAIL;

    ShowWindow( g_hWnd, nCmdShow );
    UpdateWindow( g_hWnd );

	init();

	while( uMsg.message != WM_QUIT )
	{
		if( PeekMessage( &uMsg, NULL, 0, 0, PM_REMOVE ) )
		{ 
			TranslateMessage( &uMsg );
			DispatchMessage( &uMsg );
		}
        else
		{
			g_dCurTime     = timeGetTime();
			g_fElpasedTime = (float)((g_dCurTime - g_dLastTime) * 0.001);
			g_dLastTime    = g_dCurTime;

			render();
		}
	}

	shutDown();

    UnregisterClass( "MY_WINDOWS_CLASS", winClass.hInstance );

	return uMsg.wParam;
}

//-----------------------------------------------------------------------------
// Name: WindowProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT CALLBACK WindowProc( HWND   hWnd, 
							 UINT   msg, 
							 WPARAM wParam, 
							 LPARAM lParam )
{	
	static POINT ptLastMousePosit;
	static POINT ptCurrentMousePosit;
	static bool bMousing;

    switch( msg )
	{	
        case WM_KEYDOWN:
		{
			switch( wParam )
			{
				case VK_ESCAPE:
					PostQuitMessage(0);
					break;
				case VK_SPACE: //위로 올라감
					g_aniSprite.isUp = true;
					break;
			}
		}
        break;
		
		case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);

			//메뉴화면에서 클릭시 게임 중으로 변경
			if (g_stage == 0)
			{
				MessageBox(hWnd, "▶ 게임 방법 ◀\n※ SPACE : 점프 ※\n※ 송곳과 나무를 피하여 날아보자! ※", "DirectX 과제", MB_ICONWARNING | MB_OK);
				g_stage = 1;
			}
			//게임오버 화면에서 클릭시 팝업창 띄움
			if (g_stage == 2)
			{
				MessageBox(hWnd, "▶ GAME OVER ◀\n※ 게임을 종료합니다 ※", "DirectX 과제", MB_ICONWARNING | MB_OK);
				exit(0);
			}

			ptLastMousePosit.x = ptCurrentMousePosit.x = LOWORD(lParam);
			ptLastMousePosit.y = ptCurrentMousePosit.y = HIWORD(lParam);
			bMousing = true;
		}
		break;

		case WM_LBUTTONUP:
		{
			bMousing = false;
		}
		break;

		case WM_MOUSEMOVE:
		{
			ptCurrentMousePosit.x = LOWORD(lParam);
			ptCurrentMousePosit.y = HIWORD(lParam);

			if (bMousing)
			{
				g_fSpinX -= (ptCurrentMousePosit.x - ptLastMousePosit.x);
				g_fSpinY -= (ptCurrentMousePosit.y - ptLastMousePosit.y);
			}

			ptLastMousePosit.x = ptCurrentMousePosit.x;
			ptLastMousePosit.y = ptCurrentMousePosit.y;
		}
		break;

		case WM_CLOSE:
		{
			PostQuitMessage(0);	
		}
		
        case WM_DESTROY:
		{
            PostQuitMessage(0);
		}
        break;

		default:
		{
			return DefWindowProc( hWnd, msg, wParam, lParam );
		}
		break;
	}

	

	return 0;
}


//-----------------------------------------------------------------------------
// Name: init()
// Desc: 
//-----------------------------------------------------------------------------
void init( void )
{
    g_pD3D = Direct3DCreate9( D3D_SDK_VERSION );

	D3DCAPS9 d3dCaps;
	g_pD3D->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dCaps );

    D3DDISPLAYMODE d3ddm;
    g_pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm );

    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory( &d3dpp, sizeof(d3dpp) );
	
    d3dpp.Windowed               = TRUE;
	d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat       = d3ddm.Format;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    d3dpp.PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE;

    g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWnd,
                          D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                          &d3dpp, &g_pd3dDevice );

	   
    D3DXMATRIX matProj;
    D3DXMatrixPerspectiveFovLH( &matProj, D3DXToRadian( 45.0f ), 
                                640.0f / 480.0f, 0.1f, 100.0f );
    g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );

	g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	g_aniSprite.Create(g_pd3dDevice, "character.txt");
	g_backGround.Create(g_pd3dDevice, "background.png");
	g_tree.Create(g_pd3dDevice, "tree.png");
	g_upsideTree.Create(g_pd3dDevice, "UpsideTree.png");
	g_gameOver.Create(g_pd3dDevice, "gameover.png");
	g_gameStart.Create(g_pd3dDevice, "gamestart.png");

	//사운드
	mciOpen.lpstrElementName = "mainsound.mp3";
	mciOpen.lpstrDeviceType = "mpegvideo";
	mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)&mciOpen);
	dwID = mciOpen.wDeviceID;
	mciSendCommand(dwID, MCI_PLAY, MCI_DGV_PLAY_REPEAT, (DWORD)(LPVOID)&m_mciPlayParms);
}

//-----------------------------------------------------------------------------
// Name: shutDown()
// Desc: 
//-----------------------------------------------------------------------------
void shutDown( void )
{
    if( g_pd3dDevice != NULL )
        g_pd3dDevice->Release();

    if( g_pD3D != NULL )
        g_pD3D->Release();
}

//-----------------------------------------------------------------------------
// Name: render()
// Desc: 
//-----------------------------------------------------------------------------
void render( void )
{
    g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                         D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 0 );

	g_pd3dDevice->BeginScene();

	//메뉴화면
	if(g_stage == 0)
		g_gameStart.OnUpdate(g_fElpasedTime);
	//게임 중
	if (g_stage == 1)
	{
		g_backGround.OnUpdate(g_fElpasedTime); //배경
		g_aniSprite.OnUpdate(g_fElpasedTime); //캐릭터
		g_tree.OnUpdate(g_fElpasedTime); //장애물 아래
		g_upsideTree.OnUpdate(g_fElpasedTime); //장애물 위
	}
	//게임 오버
	if (g_stage == 2)
		g_gameOver.OnUpdate(g_fElpasedTime);
	
	//장애물에 닿였을 경우 게임오버
	if ((g_aniSprite.isTrigger(g_upsideTree.m_rcCollision) == TRUE) ||
		(g_aniSprite.isTrigger(g_tree.m_rcCollision) == TRUE) ||
		(g_aniSprite.isObstacleTrigger() == TRUE)
		&& g_stage == 1)
		g_stage = 2;

	g_pd3dDevice->EndScene();

	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}