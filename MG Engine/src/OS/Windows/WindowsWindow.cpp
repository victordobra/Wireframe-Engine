#include "Windows.h"
#include "WindowsInternal.h"

#include "OSManagerInternal.h"
#include "RenderingPipelineInternal.h"
#include "RenderingPipeline.h"

#include <windows.h>
#include <tchar.h>
#include <string>
#include <istream>

#define WINDOW_CLASS_NAME _T("Desktop App")

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

static bool KeysPressed[256] = { false };

#pragma endregion

#pragma region Function Declarations

LRESULT CALLBACK WinProc(HWND WindowHWND, UINT Message, WPARAM WParam, LPARAM LParam);
int WINAPI WinMain(_In_ HINSTANCE HInstance, _In_opt_ HINSTANCE HPrevInstance, _In_ LPSTR LpCmdLine, _In_ int NCmdShow);

#pragma endregion

#pragma region Main Windows Functions

int WINAPI WinMain(_In_ HINSTANCE HInstance, _In_opt_ HINSTANCE HPrevInstance, _In_ LPSTR LpCmdLine, _In_ int NCmdShow) {
    WNDCLASSEX WCEX { };

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
    WCEX.lpszClassName = WINDOW_CLASS_NAME;
    WCEX.hIconSm = LoadIcon(WCEX.hInstance, IDI_APPLICATION);

    if (!RegisterClassEx(&WCEX)) {
        MessageBox(0, _T("Call to RegisterClassEx failed!"), _T("Error"), 0);

        return 1;
    }

    HInst = HInstance;

    ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
    ScreenHeight = GetSystemMetrics(SM_CYSCREEN);
    GameWidth = ScreenWidth;
    GameHeight = ScreenHeight;

    WinTitle = (wchar_t*)_T("Unnamed Game");
    WindowHWND = CreateWindowExW(
        0,
        WINDOW_CLASS_NAME,
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

    bool GameRunning = true;
    MSG Message;
    while (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE)) {
        if (Message.message == WM_QUIT)
            GameRunning = false;

        TranslateMessage(&Message);
        DispatchMessage(&Message);
    }
    
    return (int)Message.wParam;
}

LRESULT CALLBACK WinProc(HWND WHWND, UINT Message, WPARAM WParam, LPARAM LParam) {
    switch (Message) {
    case WM_CREATE: 
    {
        WindowHWND = WHWND;

        WinScreenColours = (unsigned long*)VirtualAlloc(0, (size_t)ScreenWidth * ScreenHeight * 4, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
        WindowHDC = GetDC(WindowHWND);

        BmpInfo.bmiHeader.biSize = sizeof(BmpInfo.bmiHeader);
        BmpInfo.bmiHeader.biWidth = ScreenWidth;
        BmpInfo.bmiHeader.biHeight = ScreenHeight;
        BmpInfo.bmiHeader.biPlanes = 1;
        BmpInfo.bmiHeader.biBitCount = 32;
        BmpInfo.bmiHeader.biCompression = BI_RGB;

        RPipeline::InitPipeline();
        OSManager::OnStart();
        break;
    }
    case WM_PAINT: 
    {
        for (int i = 0; i < 256; i++) {
            bool CurrentKeyPressed = GetAsyncKeyState(i) >> 15;

            if (!KeysPressed[i] && CurrentKeyPressed)
                OSManager::OnButtonDown(i);
            else if (KeysPressed[i] && !CurrentKeyPressed)
                OSManager::OnButtonUp(i);

            KeysPressed[i] = CurrentKeyPressed;
        }
        
        POINT MousePoint;
        GetCursorPos(&MousePoint);

        OSManager::SetMousePos(MousePoint.x, MousePoint.y);
        OSManager::OnUpdate();
        RPipeline::DrawFrame();

        UpdateWindow(WindowHWND);
        break;
    }
    case WM_CLOSE:
        DestroyWindow(WHWND);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(WHWND, Message, WParam, LParam);
        break;
    }
    
    return 0;
}

#pragma endregion

#pragma region Internal Functions

HWND Window::GetWindowHWND() {
    return WindowHWND;
}
HINSTANCE Window::GetWindowHInstance() {
    return HInst;
}

#pragma endregion

#pragma region External Functions

unsigned long*& Window::GetScreenColoursRef() {
    return WinScreenColours;
}

unsigned int& Window::GetScreenWidthRef() {
    return ScreenWidth;
}
unsigned int& Window::GetScreenHeightRef() {
    return ScreenHeight;
}
unsigned int& Window::GetGameWidthRef() {
    return GameWidth;
}
unsigned int& Window::GetGameHeightRef() {
    return GameHeight;
}

char* Window::GetWTitle() {
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
void Window::SetWTitle(const char* NewTitle) {
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

void Window::SetGameSize(unsigned int NewWidth, unsigned int NewHeight) {
    GameWidth = NewWidth;
    GameHeight = NewHeight;

    BmpInfo.bmiHeader.biWidth = GameWidth;
    BmpInfo.bmiHeader.biHeight = GameHeight;
    
    VirtualFree(WinScreenColours, 0, MEM_RELEASE);
    WinScreenColours = (unsigned long*)VirtualAlloc(0, (size_t)GameWidth * GameHeight * 4, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
}

#pragma endregion
