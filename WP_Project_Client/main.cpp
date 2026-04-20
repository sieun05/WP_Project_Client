#include "Network.h"

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <map>
#include <string>
#include <stdlib.h>
#include <random>
#include <wingdi.h>
#include <cmath>
#include <vector>
#include <algorithm>
#include <array>

#include "map.h"
#include "Player.h"
#include "PlayerManager.h"

//수정 전. main에서 함수 사용할 때 오류 안 띄우려고 임시 작성
#include "Direct2D_Player_Render.h" 

#pragma comment(lib, "Msimg32.lib")

using namespace std;

#define marginX 16
#define marginY 39

HINSTANCE g_hInst;
LPCTSTR lpszClass = _T("MyWindowClass");
LPCTSTR lpszWindowName = _T("Client");

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

int  WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_  LPSTR lpszCmdParam, _In_  int nCmdShow)
{
	HWND hWnd;
	//MSG Message;
	WNDCLASSEX WndClass;
	g_hInst = hInstance;

	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	if (!RegisterClassEx(&WndClass)) {
		MessageBox(NULL, _T("RegisterClassEx 실패"), _T("Error"), MB_OK);
		return 0;
	}

	//--- 크기 변경 가능 (기존 (1024, 768))
	hWnd = CreateWindow(
		lpszClass,
		lpszWindowName,
		WS_OVERLAPPEDWINDOW,
		100, 100,
		1280,
		720,
		NULL,
		(HMENU)NULL,
		hInstance,
		NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	MSG msg = {};
	bool running = true;

	ULONGLONG lastTime = GetTickCount64();

	NetworkInit();

	while(running){

		//1. 윈도우 메세지 처리(비동기)
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				running = false;
				break;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}// WinMain while(GetMessage(&Message, 0, 0, 0))

		NetworkUpdate();
	
	//2. 게임 로직 업데이트 (deltaTime 계산)
		ULONGLONG currentTime = GetTickCount64();
		double deltaTime = (currentTime - lastTime) / 1000.0; // 초 단위로 변환
		lastTime = currentTime;

		//게임 로직 업데이트 함수 호출 (수정. 게임 월드 클래스 만들어서 구현할지 고민)
		//UpdateGameLogic(deltaTime);
		
		for(auto& p : PlayerManager::GetInstance().GetPlayers())
			p.Update(deltaTime);

		InvalidateRect(hWnd, NULL, FALSE); // 화면 갱신 요청

		//(선택) CPU쉬게하기
		Sleep(1); // 1ms 쉬기 (필요에 따라 조절)
		
	}// WinMain while(running)

	NetworkRelease();
	return (int)msg.wParam;
}

//전역변수 선언

//맵, 플레이어매니저
extern TileMap g_map;
extern PlayerManager playerManager;

//윈도우 크기, 중앙 좌표 변수
int width{};
int height{};
double rect_width{};
double rect_height{};
int centerx{};
int centery{};

//내 플레이어 id 변수 (playerManager에서 타 플레이어와 구분)
int myPlayerId{};

//bitmap 물 렌더링 변수 (맵 클래스에 넣어버릴까?)
int water_sel{};
int water_delay{};

//screen 변수 (0 메인화면, 1 게임화면) 이건 게임월드 클래스 만들어서 구현할지 생각하기
//일단 플레이화면부터 만들자 (비활성화)
//int screen{};

//타이머 변수
int min{};
int sec{};

//배경 좌표 변수, 카메라 위치 (수정. 검색-카메라 시작위치)
int bx{};
int by{};

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{

	HDC hdc;
	PAINTSTRUCT ps;

	/*HDC hdc, mdc, memdc, memdc2;
	HDC maskDC, tempDC;
	HGDIOBJ oldMask;
	PAINTSTRUCT ps;
	HBITMAP hbitmap;*/
	static RECT clientRect;
	TCHAR txt[100];

	//알파블렌딩 변수
	BLENDFUNCTION blend{};


	switch (iMessage) {
	case WM_CREATE:
	{

		InitDirect2D(hWnd);
		CoInitialize(NULL); // WIC용
		HRESULT hr = CoCreateInstance(
			CLSID_WICImagingFactory,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&g_pWICFactory));

		if (FAILED(hr)) {
			MessageBox(hWnd, _T("Failed to create WIC Imaging Factory"), _T("Error"), MB_OK | MB_ICONERROR);
			return -1;
		}

		//수정. 프로젝트에 비트맵 추가 안 함
		LoadD2DBitmapFromFile(g_pRenderTarget, g_pWICFactory, L"비트맵\\타일\\벽.png", &g_WallBitmap);
		LoadD2DBitmapFromFile(g_pRenderTarget, g_pWICFactory, L"비트맵\\타일\\땅3.png", &ground[0]);
		LoadD2DBitmapFromFile(g_pRenderTarget, g_pWICFactory, L"비트맵\\타일\\풀땅.png", &ground[1]);
		LoadD2DBitmapFromFile(g_pRenderTarget, g_pWICFactory, L"비트맵\\타일\\주황풀땅3.png", &ground[2]);
		LoadD2DBitmapFromFile(g_pRenderTarget, g_pWICFactory, L"비트맵\\타일\\물.png", &water);

		LoadD2DBitmapFromFile(g_pRenderTarget, g_pWICFactory, L"비트맵\\플레이어\\1.png", &PlayerBmp);

		//맵 초기화 (수정. 서버에서 초기 맵 데이터 받아서 저장)
		//map.InitMap();

		//윈도우 창 설정값 변수에 저장
		GetClientRect(hWnd, &clientRect);
		width = clientRect.right - clientRect.left;
		height = clientRect.bottom - clientRect.top;

		//윈도우창 크기에 따라 윈도우에 나타나는 타일 개수 (가로, 세로)
		rect_width = width / CELL_SIZE;
		rect_height = height / CELL_SIZE;

		//중앙좌표
		centerx = (clientRect.left + clientRect.right) / 2;
		centery = (clientRect.top + clientRect.bottom) / 2;

		//카메라 시작위치(수정. 서버에서 플레이어 위치 받아서 시작, 카메라 위치는 플레이어 좌표값에 종속)
		bx = MAP_PIXEL_WIDTH / 2 - width / 2;
		by = MAP_PIXEL_HEIGHT / 2 - height / 2;

		// 화면 밖으로 벗어나지 않게 보정 
		// (수정. 플레이어 좌표값에 따라 카메라가 화면 벗어나지 않게 보정. 
		// 플레이어 클래스에 넣을지 고민)
		if (bx < 0) bx = 0;
		if (by < 0) by = 0;
		if (bx > MAP_PIXEL_WIDTH - width) bx = MAP_PIXEL_WIDTH - width;
		if (by > MAP_PIXEL_HEIGHT - height) by = MAP_PIXEL_HEIGHT - height;

		//알파블렌딩, 그라디언트 스톱 생성
		g_pRenderTarget->CreateGradientStopCollection(stops, 2, &pGradientStops);

		//중심 위치 설정 (수정. 뭔지 모르겠음)
		float centerX = clientRect.right / 2.0f;
		float centerY = clientRect.bottom / 2.0f;
		float radiusX = clientRect.right / 1.2f;
		float radiusY = clientRect.bottom / 1.2f;

		//브러시 생성
		g_pRenderTarget->CreateRadialGradientBrush(
			D2D1::RadialGradientBrushProperties(
				D2D1::Point2F(centerX, centerY),
				D2D1::Point2F(0, 0),
				radiusX, radiusY
			),
			pGradientStops,
			&pRadialBrush
		);

		SetWindowLong(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);

		SetWindowPos(hWnd, NULL, 0, 0, 0, 0,
			SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

		RECT wr, cr;
		GetWindowRect(hWnd, &wr);
		GetClientRect(hWnd, &cr);

		TCHAR buf[200];
		wsprintf(buf, _T("W: %d H: %d / CW: %d CH: %d"),
			wr.right - wr.left,
			wr.bottom - wr.top,
			cr.right - cr.left,
			cr.bottom - cr.top);

		MessageBox(hWnd, buf, _T("SIZE"), MB_OK);

		break;
	} //WM_CREATE switch

	case WM_KEYDOWN:
	{
		switch (wParam) {
		case 'a':
			bx -= 10; //카메라 이동 (수정. 플레이어 이동에 따라 카메라가 따라오도록 수정)
			break;
		case 'd':
			bx += 10;
			break;
		case 'w':
			by -= 10;
			break;
		case 's':
			by += 10;
			break;
		case 'Q':
		case 'q':
			//게임 종료 (수정. 타이틀 화면에서만 종료, 게임플레이 중에는 확인창 띄우기)
			ReleaseD2DBitmaps();
			PostQuitMessage(0);
			break;

			//case 'E':
			//case 'e':
			//	//상호작용 (수정. 상호작용 가능한 오브젝트가 있을 때만 작동)
			//	int playerWorldX = bx + p.x;
			//	int playerWorldY = by + p.y;
			//	DWORD now = GetTickCount64();
			//	int slot = InvenSelec_Nor - 1;
			//	break;
		} //WM_KEYDOWN switch - wParam switch

		break;
	} //WM_KEYDOWN switch
	case WM_PAINT:
	{


		//클라이언트 값 가져오기
		GetClientRect(hWnd, &clientRect);

		//렌더링을 위한 HDC설정 (더블버퍼링)
		hdc = BeginPaint(hWnd, &ps);
		/*mdc = CreateCompatibleDC(hdc);
		hbitmap = CreateCompatibleBitmap(hdc, clientRect.right, clientRect.bottom);
		SelectObject(mdc, (HBITMAP)hbitmap);
		memdc = CreateCompatibleDC(mdc);
		memdc2 = CreateCompatibleDC(mdc);
		FillRect(mdc, &clientRect, (HBRUSH)GetStockObject(WHITE_BRUSH));*/

		//Direct2D 렌더링 시작
		g_pRenderTarget->BeginDraw();
		g_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White)); // 배경색

		//게임상태마다 렌더링 다름	(수정. 타이틀, 게임플레이, 게임오버 / 인벤창, 제작창, 요리창)

		//게임플레이 렌더링 (수정)
		//맵 렌더링
		//플레이어 렌더링 
		// (각 오브젝트를 루프 돌려 렌더링하는 것이 아니라 매니저에서 렌더링한다. 
		// - 따로 매니저 구조로 관리)
		//UI, HUD는 오브젝트별 직접 렌더링할 수 있다.
		// 몬스터, 플레이어 깊이정렬이 필요할 때는 타일 + 오브젝트를 리스트로 만들어서 정렬한 뒤 렌더링한다)
		// 맵이 여러 개라면 맵 매니저가 필요해진다. (씬 전환)

		g_map.Render(bx, by);
		
		for (auto& p : PlayerManager::GetInstance().GetPlayers())
			p.Render();

		//수정. 지금까지 그린 것 출력
		g_pRenderTarget->FillRectangle(
			D2D1::RectF(0, 0, (FLOAT)clientRect.right, (FLOAT)clientRect.bottom),
			pRadialBrush
		);

		//여기에 UI, HUD 렌더링 추가 (수정)
		//체력, 허기, 마나, 스태미나, 인벤토리, 제작대, 요리대, 도전과제 등

		//Direct2D 렌더링 끝
		g_pRenderTarget->EndDraw();

		//GDI 렌더링 끝, 메모리 해제
		/*DeleteDC(memdc);
		DeleteDC(memdc2);
		DeleteDC(mdc);
		DeleteObject(hbitmap);
		*/
		EndPaint(hWnd, &ps);
		break;
	} //WM_PAINT switch

	case WM_DESTROY:
	{
		//메모리 해제 필수
		//Direct2D 메모리 해제 함수. 수정. 관리하기 편하게 함수로 정리.
		CleanupDirect2D();
		/*if (g_WallBitmap) g_WallBitmap->Release();
		if (g_pWICFactory) g_pWICFactory->Release();
		for (int i = 0; i < 3; ++i)
			if (ground[i]) ground[i]->Release();
		if (water) water->Release();*/

		CoUninitialize();
		ReleaseD2DBitmaps();

		PostQuitMessage(0);
		return 0;
	}//WM_DESTROY switch
	return DefWindowProc(hWnd, iMessage, wParam, lParam);
	} //WndProc iMessage switch 
} //WndProc