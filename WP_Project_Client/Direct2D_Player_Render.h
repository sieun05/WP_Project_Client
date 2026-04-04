#pragma once
#include <d2d1.h>
#include <wincodec.h>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "windowscodecs.lib")

// Direct2D 전역 변수
extern ID2D1Factory* g_pD2DFactory;
extern ID2D1HwndRenderTarget* g_pRenderTarget;
 
extern IWICImagingFactory* g_pWICFactory;
extern ID2D1Bitmap* g_WallBitmap;
extern ID2D1Bitmap* ground[3];
extern ID2D1Bitmap* water;
extern ID2D1Bitmap* mainscreen;
extern ID2D1Bitmap* endscreen;
extern ID2D1Bitmap* pclick[3];
extern ID2D1Bitmap* player_died;
extern ID2D1Bitmap* hp_bar;
extern ID2D1Bitmap* mp_bar;
extern ID2D1Bitmap* stmn_bar;
extern ID2D1Bitmap* PlayerBmp;
 
extern ID2D1Bitmap* craftUI;
extern ID2D1Bitmap* cookUI;
 
extern ID2D1Bitmap* arrowBmp;
extern ID2D1Bitmap* DaggerAttackBmp;
extern ID2D1Bitmap* LongswordAttackBmp;
extern ID2D1Bitmap* fireBmp;
extern ID2D1Bitmap* waterEffectBmp;
 
extern ID2D1Bitmap* Itembmp;
extern ID2D1Bitmap* Numbmp;
extern ID2D1Bitmap* Arrowbmp; //??
extern ID2D1Bitmap* itemDescBmp;
 
extern ID2D1Bitmap* InvenExpanded;
extern ID2D1Bitmap* InvenNormal;
extern ID2D1Bitmap* InvenExpandednum;
extern ID2D1Bitmap* InvenNormalnum;
extern ID2D1Bitmap* SelecInven_Ex;
extern ID2D1Bitmap* SelecInven_Nor;
 
extern ID2D1Bitmap* bossBmp;
 
extern ID2D1Bitmap* mon1;
extern ID2D1Bitmap* mon2;

//알파블랜딩
extern ID2D1RadialGradientBrush* pRadialBrush;
extern ID2D1GradientStopCollection* pGradientStops;

extern D2D1_GRADIENT_STOP stops[2];


// 초기화 함수 선언
bool InitDirect2D(HWND hWnd);
void CleanupDirect2D();
bool LoadD2DBitmapFromFile(
    ID2D1RenderTarget* rt,
    IWICImagingFactory* wicFactory,
    const wchar_t* filename,
    ID2D1Bitmap** outBitmap);
void ReleaseD2DBitmaps();

