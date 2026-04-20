#include "Direct2D_Player_Render.h"

// Direct2D 전역 변수
ID2D1Factory* g_pD2DFactory = nullptr;
ID2D1HwndRenderTarget* g_pRenderTarget = nullptr;

IWICImagingFactory* g_pWICFactory = nullptr;
ID2D1Bitmap* g_WallBitmap = nullptr;
ID2D1Bitmap* ground[3] = {};
ID2D1Bitmap* water = nullptr;
ID2D1Bitmap* mainscreen = nullptr;
ID2D1Bitmap* endscreen = nullptr;
ID2D1Bitmap* pclick[3] = {};
ID2D1Bitmap* player_died = nullptr;
ID2D1Bitmap* hp_bar = nullptr;
ID2D1Bitmap* mp_bar = nullptr;
ID2D1Bitmap* stmn_bar = nullptr;
ID2D1Bitmap* PlayerBmp = nullptr;

ID2D1Bitmap* craftUI = nullptr;
ID2D1Bitmap* cookUI = nullptr;

ID2D1Bitmap* arrowBmp = nullptr;
ID2D1Bitmap* DaggerAttackBmp = nullptr;
ID2D1Bitmap* LongswordAttackBmp = nullptr;
ID2D1Bitmap* fireBmp = nullptr;
ID2D1Bitmap* waterEffectBmp = nullptr;

ID2D1Bitmap* Itembmp = nullptr;
ID2D1Bitmap* Numbmp = nullptr;
ID2D1Bitmap* Arrowbmp = nullptr; //??
ID2D1Bitmap* itemDescBmp = nullptr;

ID2D1Bitmap* InvenExpanded = nullptr;
ID2D1Bitmap* InvenNormal = nullptr;
ID2D1Bitmap* InvenExpandednum = nullptr;
ID2D1Bitmap* InvenNormalnum = nullptr;
ID2D1Bitmap* SelecInven_Ex = nullptr;
ID2D1Bitmap* SelecInven_Nor = nullptr;

ID2D1Bitmap* bossBmp = nullptr;

ID2D1Bitmap* mon1 = nullptr;
ID2D1Bitmap* mon2 = nullptr;

//알파블랜딩
ID2D1RadialGradientBrush* pRadialBrush = nullptr;
ID2D1GradientStopCollection* pGradientStops = nullptr;

D2D1_GRADIENT_STOP stops[2] = {
    { 0.0f, D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.0f) },  // 중심: 완전 투명
    { 1.0f, D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.9f) }   // 외곽: 어두움 (알파=0.8)
};

bool InitDirect2D(HWND hWnd) {
    HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &g_pD2DFactory);
    if (FAILED(hr)) return false;

    RECT rc;
    GetClientRect(hWnd, &rc);

    if (rc.right == 0 || rc.bottom == 0) {
        //
        return false;
    }

    D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties();
    D2D1_HWND_RENDER_TARGET_PROPERTIES hwndProps =
        D2D1::HwndRenderTargetProperties(hWnd,
            D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top));

    hr = g_pD2DFactory->CreateHwndRenderTarget(
        props,
        hwndProps,
        &g_pRenderTarget);

    return SUCCEEDED(hr);
}

void CleanupDirect2D() {
    if (g_pRenderTarget) g_pRenderTarget->Release();
    if (g_pD2DFactory) g_pD2DFactory->Release();
}

bool LoadD2DBitmapFromFile(ID2D1RenderTarget* rt, IWICImagingFactory* wicFactory, const wchar_t* filename, ID2D1Bitmap** outBitmap) {
    IWICBitmapDecoder* decoder = nullptr;
    IWICBitmapFrameDecode* frame = nullptr;
    IWICFormatConverter* converter = nullptr;

    HRESULT hr = wicFactory->CreateDecoderFromFilename(filename, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder);
    if (FAILED(hr)) return false;

    hr = decoder->GetFrame(0, &frame);
    if (FAILED(hr)) return false;

    hr = wicFactory->CreateFormatConverter(&converter);
    if (FAILED(hr)) return false;

    hr = converter->Initialize(frame, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone,
        NULL, 0.0, WICBitmapPaletteTypeMedianCut);
    if (FAILED(hr)) return false;

    hr = rt->CreateBitmapFromWicBitmap(converter, NULL, outBitmap);
    if (FAILED(hr)) return false;

    // 해제
    if (converter) converter->Release();
    if (frame) frame->Release();
    if (decoder) decoder->Release();

    return true;
}

void ReleaseD2DBitmaps() {
    // 타일
    if (g_WallBitmap)        g_WallBitmap->Release();
    for (int i = 0; i < 3; ++i)
        if (ground[i])       ground[i]->Release();
    if (water)               water->Release();

    // 배경 화면
    if (mainscreen)          mainscreen->Release();
    if (endscreen)           endscreen->Release();

    // 도전과제
    for (int i = 0; i < 3; ++i)
        if (pclick[i])       pclick[i]->Release();

    // 플레이어
    if (player_died)         player_died->Release();
    if (hp_bar)              hp_bar->Release();
    if (mp_bar)              mp_bar->Release();
    if (stmn_bar)            stmn_bar->Release();
    if (PlayerBmp)           PlayerBmp->Release();

    // UI
    if (craftUI)             craftUI->Release();
    if (cookUI)              cookUI->Release();

    // 무기 / 마법 / 투사체
    if (arrowBmp)            arrowBmp->Release();
    if (DaggerAttackBmp)     DaggerAttackBmp->Release();
    if (LongswordAttackBmp)  LongswordAttackBmp->Release();
    if (fireBmp)             fireBmp->Release();
    if (waterEffectBmp)      waterEffectBmp->Release();

    // 아이템
    if (itemDescBmp)         itemDescBmp->Release();
    if (Itembmp)             Itembmp->Release();
    if (Numbmp)              Numbmp->Release();

    // 인벤토리
    if (InvenExpanded)       InvenExpanded->Release();
    if (InvenNormal)         InvenNormal->Release();
    if (InvenExpandednum)    InvenExpandednum->Release();
    if (InvenNormalnum)      InvenNormalnum->Release();
    if (SelecInven_Ex)       SelecInven_Ex->Release();
    if (SelecInven_Nor)      SelecInven_Nor->Release();
}
