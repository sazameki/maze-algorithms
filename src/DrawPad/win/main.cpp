//
//  main.cpp
//
//  Created by Satoshi Numata on 2018/12/28.
//  License: Public domain.
//

#include <windows.h>
#include "Resource.h"
#include "Drawing.hpp"


void DrawMain();


// 変数
HINSTANCE   hAppInstance;   // アプリケーションのインスタンス
HWND        hWindow;        // ウィンドウ
HANDLE      hThread;        // 描画用スレッド
HDC         hDibDC;         // DIB描画用のデバイスコンテキスト


void FinishDrawing()
{
    InvalidateRect(hWindow, NULL, FALSE);
    PAINTSTRUCT ps;
    HDC hDC = BeginPaint(hWindow, &ps);
    BitBlt(hDC, 0, 0, 640, 480, hDibDC, 0, 0, SRCCOPY);
    EndPaint(hWindow, &ps);
    ReleaseDC(hWindow, hDC);
}

void Sleep(float seconds)
{
    // FIXME: ::Sleep(DWORD)でスリープさせると、直前の描画が画面に反映されない場合があるのを修正する。
    Sleep((DWORD)(seconds * 1000));
}

/**
    描画用スレッド関数
 */
static DWORD WINAPI ThreadProc(LPVOID arg)
{
    // ビットマップの構成
    BITMAPINFO  bmpInfo;
    bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfo.bmiHeader.biWidth = 640;
    bmpInfo.bmiHeader.biHeight = -480;
    bmpInfo.bmiHeader.biPlanes = 1;
    bmpInfo.bmiHeader.biBitCount = 32;
    bmpInfo.bmiHeader.biCompression = BI_RGB;

    // DIBを生成してhDibDCに割り当てる
    HDC hDC = GetDC(hWindow);
    hDibDC = CreateCompatibleDC(hDC);
    HBITMAP hBitmap = CreateDIBSection(hDC, &bmpInfo, DIB_RGB_COLORS, (void**)&gpBuffer, NULL, 0);
    SelectObject(hDibDC, hBitmap);
    ReleaseDC(hWindow, hDC);

    // 画面のクリア
    gIsBatchDrawing = true;
    Clear(kColorBlack);
    gIsBatchDrawing = false;

    // キーボード・マウス関係の変数の初期化
    gKeyData = 0;
    gIsMouseDown = false;

    // DrawPadのメイン関数の実行
    DrawMain();

    // クリーンアップ
    DeleteObject(hBitmap);
    DeleteDC(hDibDC);
    return 0;
}

/**
    バージョン情報ボックスのメッセージハンドラ
 */
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

/**
    イベント・ハンドラ
 */
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // TODO: マウスの処理を追加する。
    switch (message)
    {
    case WM_CREATE:
    {
        hThread = CreateThread(NULL, 0, ThreadProc, 0, 0, NULL);
        break;
    }
    case WM_KEYDOWN:
        switch (wParam) {
        case VK_LEFT:
            gKeyData |= kKeyLeftArrow;
            break;
        case VK_RIGHT:
            gKeyData |= kKeyRightArrow;
            break;
        case VK_UP:
            gKeyData |= kKeyUpArrow;
            break;
        case VK_DOWN:
            gKeyData |= kKeyDownArrow;
            break;
        case VK_RETURN:
            gKeyData |= kKeyReturn;
            break;
        case VK_SPACE:
            gKeyData |= kKeySpace;
            break;
        case 'A':
            gKeyData |= kKeyA;
            break;
        case 'S':
            gKeyData |= kKeyS;
            break;
        case 'D':
            gKeyData |= kKeyD;
            break;
        case 'W':
            gKeyData |= kKeyW;
            break;
        }
        break;
    case WM_KEYUP:
        switch (wParam) {
        case VK_LEFT:
            gKeyData &= ~kKeyLeftArrow;
            break;
        case VK_RIGHT:
            gKeyData &= ~kKeyRightArrow;
            break;
        case VK_UP:
            gKeyData &= ~kKeyUpArrow;
            break;
        case VK_DOWN:
            gKeyData &= ~kKeyDownArrow;
            break;
        case VK_RETURN:
            gKeyData &= ~kKeyReturn;
            break;
        case VK_SPACE:
            gKeyData &= ~kKeySpace;
            break;
        case 'A':
            gKeyData &= ~kKeyA;
            break;
        case 'S':
            gKeyData &= ~kKeyS;
            break;
        case 'D':
            gKeyData &= ~kKeyD;
            break;
        case 'W':
            gKeyData &= ~kKeyW;
            break;
        }
        break;
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case IDM_ABOUT:
            DialogBox(hAppInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

/**
    ウィンドウ・クラスの登録
 */
ATOM RegisterWindowClass(LPCTSTR className)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hAppInstance;
    wcex.hIcon          = LoadIcon(hAppInstance, MAKEINTRESOURCE(IDI_MAZE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MAZE);
    wcex.lpszClassName  = className;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

/**
    ウィンドウを生成します。
 */
bool MakeWindow(LPCTSTR className, int nCmdShow)
{
   // 最大化できないウィンドウスタイル
   DWORD winStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

   // タイトルバーなどのサイズを考慮したウィンドウサイズの計算
   RECT rect = { 0, 0, 640, 480 };
   AdjustWindowRect(&rect, winStyle, true);

   // ウィンドウの生成
   hWindow = CreateWindowW(
       className, TEXT("Maze"), winStyle,
       CW_USEDEFAULT, CW_USEDEFAULT,
       rect.right - rect.left, rect.bottom - rect.top,
       nullptr, nullptr, hAppInstance, nullptr);

   if (!hWindow)
   {
      return false;
   }

   // ウィンドウの表示
   ShowWindow(hWindow, nCmdShow);
   UpdateWindow(hWindow);

   return true;
}

/**
    エントリ・ポイント
 */
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // アプリケーション・インスタンスの保持
    hAppInstance = hInstance;

    // グローバル文字列を初期化する
    const wchar_t* windowClassName = TEXT("Maze Window");
    RegisterWindowClass(windowClassName);

    // ウィンドウの生成
    if (!MakeWindow(windowClassName, nCmdShow))
    {
        return FALSE;
    }

    // メインのメッセージループ
    MSG msg;
    HACCEL hAccelTable = LoadAccelerators(hAppInstance, MAKEINTRESOURCE(IDC_MAZE));
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}

