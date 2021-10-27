#include "WindowsWindow.h"
#include "OSManager.h"
#include "OSManagerInternal.h"
#include <string>
#include <windows.h>
#include <tchar.h>
#include <string>
#include <codecvt>

#define WindowClassName "DesktopApp"

#pragma region Variables

static HINSTANCE HInst;
static HDC WindowHDC;
static HWND WindowHWND;
static std::string WinTitle;

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
    WCEX.lpszClassName = WindowClassName;
    WCEX.hIconSm = LoadIcon(WCEX.hInstance, IDI_APPLICATION);

    if (!RegisterClassEx(&WCEX)) {
        MessageBox(0, "Call to RegisterClassEx failed!", "Error", 0);

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
        WindowClassName,
        "Unnamed Game",
        WS_POPUP,
        0, 0,
        ScreenWidth, ScreenHeight,
        0,
        0,
        HInstance,
        0
    );

    if (!WindowHWND) {
        MessageBox(0, "Call to CreateWindow failed!", "Error", 0);

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

LRESULT CALLBACK WinProc(HWND WindowHWND, UINT Message, WPARAM WParam, LPARAM LParam) {
    switch (Message) {
    case WM_CREATE:
        WinScreenColours = (unsigned long*)VirtualAlloc(0, ScreenWidth * ScreenHeight * 4, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
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

        UpdateWindow(WindowHWND);
        break;
    case WM_CLOSE:
        DestroyWindow(WindowHWND);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_KEYDOWN:
        break;
    case WM_MOUSEMOVE:
        break;
    default:
        return DefWindowProc(WindowHWND, Message, WParam, LParam);
        break;
    }
    
    return 0;
}

#pragma endregion

#pragma region Refference Functions

unsigned long*& WindowsWin::GetScreenColoursRef() {
    return WinScreenColours;
}
unsigned int& WindowsWin::GetScreenWidthRef() {
    return ScreenWidth;
}
unsigned int& WindowsWin::GetScreenHeightRef() {
    return ScreenHeight;
}
unsigned int& WindowsWin::GetGameWidthRef() {
    return GameWidth;
}
unsigned int& WindowsWin::GetGameHeightRef() {
    return GameHeight;
}

std::string WindowsWin::GetWTitle() {
    return WinTitle;
}
void WindowsWin::SetWTitle(const char* NewTitle) {
    WinTitle = NewTitle;
    SetWindowText(WindowHWND, NewTitle);
}

void WindowsWin::SetGameSize(unsigned int NewWidth, unsigned int NewHeight) {
    GameWidth = NewWidth;
    GameHeight = NewHeight;

    BmpInfo.bmiHeader.biWidth = GameWidth;
    BmpInfo.bmiHeader.biHeight = GameHeight;
    
    VirtualFree(WinScreenColours, 0, MEM_RELEASE);
    WinScreenColours = (unsigned long*)VirtualAlloc(0, GameWidth * GameHeight * 4, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
}

#pragma endregion
