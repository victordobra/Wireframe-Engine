#include "Windows.h"
#include "OSManager.h"
#include "OSManagerInternal.h"
#include <windows.h>
#include <tchar.h>
#include <string>
#include <istream>

#define WindowClassName "DesktopApp"

#pragma region Variables

static HINSTANCE HInst;
static HDC WindowHDC;
static HWND WindowHWND;

#ifdef UNICODE
wchar_t* WinTitle;
#else
char* WinTitle;
#endif

unsigned int ScreenWidth;
unsigned int ScreenHeight;
unsigned int GameWidth;
unsigned int GameHeight;
unsigned long* WinScreenColours = nullptr;
BITMAPINFO BmpInfo;

#pragma endregion

#pragma region Function declarations

LRESULT CALLBACK WinProc(HWND WindowHWND, UINT Message, WPARAM WParam, LPARAM LParam);
int WINAPI WinMain(_In_ HINSTANCE HInstance, _In_opt_ HINSTANCE HPrevInstance, _In_ LPSTR LpCmdLine, _In_ int NCmdShow);

#pragma endregion

#pragma region Main windows functions

int WINAPI WinMain(_In_ HINSTANCE HInstance, _In_opt_ HINSTANCE HPrevInstance, _In_ LPSTR LpCmdLine, _In_ int NCmdShow) {
    //Create the window WCEX, set all of its data and register it
    WNDCLASSEX WCEX;

    WCEX.cbSize = sizeof(WNDCLASSEX);
    WCEX.style = CS_HREDRAW | CS_VREDRAW;
    WCEX.lpfnWndProc = WinProc;
    WCEX.cbClsExtra = 0;
    WCEX.cbWndExtra = 0;
    WCEX.hInstance = HInstance;
    WCEX.hIcon = LoadIcon(WCEX.hInstance, IDI_APPLICATION);
    WCEX.hCursor = LoadCursor(0, IDC_ARROW);
    WCEX.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    WCEX.lpszMenuName = 0;
    WCEX.lpszClassName = _T(WindowClassName);
    WCEX.hIconSm = LoadIcon(WCEX.hInstance, IDI_APPLICATION);

    if (!RegisterClassEx(&WCEX)) {
        MessageBox(0, _T("Call to RegisterClassEx failed!"), _T("Error"), 0);

        return 1;
    }

    HInst = HInstance;

    //Create the window
    ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
    ScreenHeight = GetSystemMetrics(SM_CYSCREEN);
    GameWidth = ScreenWidth;
    GameHeight = ScreenHeight;

    WindowHWND = CreateWindowEx(
        0,
        _T(WindowClassName),
        _T("Unnamed Game"),
        WS_POPUP,
        0, 0,
        ScreenWidth, ScreenHeight,
        0,
        0,
        HInstance,
        0
    );

    if (!WindowHWND) {
        MessageBox(0, _T("Call to CreateWindow failed!"), _T("Error"), 0);

        return 1;
    }
    
    ShowWindow(WindowHWND, NCmdShow);
    UpdateWindow(WindowHWND);

    // Main message loop
    MSG Message;
    while (GetMessage(&Message, NULL, 0, 0)) {
        TranslateMessage(&Message);
        DispatchMessage(&Message);
    }
    
    return (int)Message.wParam;
}

LRESULT CALLBACK WinProc(HWND WHWND, UINT Message, WPARAM WParam, LPARAM LParam) {
    switch (Message) {
    case WM_CREATE:
        WindowHWND = WHWND;
        
        WinScreenColours = (unsigned long*)VirtualAlloc(0, (size_t)ScreenWidth * ScreenHeight * 4, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
        WindowHDC = GetDC(WindowHWND);

        BmpInfo.bmiHeader.biSize = sizeof(BmpInfo.bmiHeader);
        BmpInfo.bmiHeader.biWidth = ScreenWidth;
        BmpInfo.bmiHeader.biHeight = ScreenHeight;
        BmpInfo.bmiHeader.biPlanes = 1;
        BmpInfo.bmiHeader.biBitCount = 32;
        BmpInfo.bmiHeader.biCompression = BI_RGB;

        OSManager::OnStart();
        break;
    case WM_PAINT:
        OSManager::OnUpdate();
        StretchDIBits(WindowHDC, 0, 0, ScreenWidth, ScreenHeight, 0, 0, GameWidth, GameHeight, WinScreenColours, &BmpInfo, DIB_RGB_COLORS, SRCCOPY);

        UpdateWindow(WHWND);
        break;
    case WM_CLOSE:
        DestroyWindow(WHWND);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_KEYDOWN:
        break;
    case WM_MOUSEMOVE:
        break;
    default:
        return DefWindowProc(WHWND, Message, WParam, LParam);
        break;
    }
    
    return 0;
}

#pragma endregion

#pragma region External Functions

unsigned long*& WindowsWindow::GetScreenColoursRef() {
    return WinScreenColours;
}
unsigned int& WindowsWindow::GetScreenWidthRef() {
    return ScreenWidth;
}
unsigned int& WindowsWindow::GetScreenHeightRef() {
    return ScreenHeight;
}
unsigned int& WindowsWindow::GetGameWidthRef() {
    return GameWidth;
}
unsigned int& WindowsWindow::GetGameHeightRef() {
    return GameHeight;
}

char* WindowsWindow::GetWTitle() {
#ifdef UNICODE
    const size_t TSize = wcslen(WinTitle) + 1;
    char* CharWinTitle = new char[TSize];
    size_t TConvCount;

    wcstombs_s(&TConvCount, CharWinTitle, TSize, WinTitle, TSize);

    if (TConvCount != TSize)
        throw std::runtime_error("Not enough characters converted!");

    return CharWinTitle;
#else
    return WinTitle;
#endif
}
void WindowsWindow::SetWTitle(const char* NewTitle) {
#ifdef UNICODE
    delete[] WinTitle;

    const size_t TSize = strlen(NewTitle) + 1;
    WinTitle = new wchar_t[TSize];
    size_t TConvCount;

    mbstowcs_s(&TConvCount, WinTitle, TSize, NewTitle, TSize);

    if (TConvCount != TSize)
        throw std::runtime_error("Not enough characters converted!");

    SetWindowText(WindowHWND, WinTitle);
#else
    WinTitle = (char*)NewTitle;

    SetWindowText(WindowHWND, WinTitle);
#endif
}

void WindowsWindow::SetGameSize(unsigned int NewWidth, unsigned int NewHeight) {
    GameWidth = NewWidth;
    GameHeight = NewHeight;

    BmpInfo.bmiHeader.biWidth = GameWidth;
    BmpInfo.bmiHeader.biHeight = GameHeight;
    
    VirtualFree(WinScreenColours, 0, MEM_RELEASE);
    WinScreenColours = (unsigned long*)VirtualAlloc(0, (size_t)GameWidth * GameHeight * 4, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
}

#pragma endregion
