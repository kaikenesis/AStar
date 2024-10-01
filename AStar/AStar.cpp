// AStar.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "AStar.h"
#include "AStarSystem.h"
#include <windowsx.h>

#define MAX_LOADSTRING 100

// DoubleBuffer
void DrawBitmap(HWND hWnd, HDC hdc);
void DrawBitmapDoubleBuffering(HWND hWnd, HDC hdc);
void DestroyBitmap();

RECT rectView;
HBITMAP hDoubleBufferImage;

#define TIMER_ASTAR 1

// dfs로 도착점과, 출발점에서 이동가능한 경로에 코스트를 각자 매기고, 두 코스트의 합이 낮은곳으로 이동하면서 최단경로탐색

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_ASTAR, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ASTAR));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ASTAR));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_ASTAR);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static POINT mousePos = { 0,0 };
    static bool bRBPressed = false;

    switch (message)
    {
    case WM_CREATE:
        GetClientRect(hWnd, &rectView);
        CreateBlock(rectView);
        SetTimer(hWnd, TIMER_ASTAR, 33, NULL);

        mousePos.x = GET_X_LPARAM(lParam);
        mousePos.y = GET_Y_LPARAM(lParam);

        break;
    case WM_CHAR:
        switch (wParam)
        {
        case 49: SetWallMode(Create);   break;
        case 50: SetWallMode(Destroy);  break;
        }

        break;
    case WM_LBUTTONDOWN:
        ChangeBlockType(mousePos.x, mousePos.y, DrawPoint);
        CalculateCost();

        break;
    case WM_RBUTTONDOWN:
        bRBPressed = true;
        
        break;
    case WM_RBUTTONUP:
        bRBPressed = false;
        break;
    case WM_TIMER:
        switch (wParam)
        {
        case TIMER_ASTAR:
        {
            if (bRBPressed == true)
            {
                ChangeBlockType(mousePos.x, mousePos.y, DrawWall);
                CalculateCost();
            }
        }
            InvalidateRgn(hWnd, NULL, FALSE);
            break;
        }

        break;
    case WM_MOUSEMOVE:
        mousePos.x = GET_X_LPARAM(lParam);
        mousePos.y = GET_Y_LPARAM(lParam);
        break;

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...

            DrawBitmapDoubleBuffering(hWnd, hdc);

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        DeleteBlock();
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
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

void DrawBitmap(HWND hWnd, HDC hdc)
{
    // drawMap
    {
        UpdateBlock(hdc);
    }
}

void DrawBitmapDoubleBuffering(HWND hWnd, HDC hdc)
{
    HDC hDoubleBufferDC;
    HBITMAP hOldDoubleBufferBitmap;

    hDoubleBufferDC = CreateCompatibleDC(hdc);
    if (hDoubleBufferImage == NULL)
    {
        hDoubleBufferImage = CreateCompatibleBitmap(hdc, rectView.right, rectView.bottom);
    }
    hOldDoubleBufferBitmap = (HBITMAP)SelectObject(hDoubleBufferDC, hDoubleBufferImage);

    DrawBitmap(hWnd, hDoubleBufferDC);

    BitBlt(hdc, 0, 0, rectView.right, rectView.bottom, hDoubleBufferDC, 0, 0, SRCCOPY);
    SelectObject(hDoubleBufferDC, hOldDoubleBufferBitmap);
    DeleteDC(hDoubleBufferDC);
}

void DestroyBitmap()
{
    DeleteObject(hDoubleBufferImage);
}

