#include "Windows.h"
#include "WindowsInternal.h"

#include "OSManagerInternal.h"
#include "RenderingPipeline.h"

#include <windows.h>
#include <tchar.h>
#include <string>
#include <istream>

#define WINDOW_CLASS_NAME _T("Desktop App")

static HINSTANCE hInst;
static HDC windowHDC;
static HWND windowHWND;

#ifdef UNICODE
static wchar_t* winTitle;
#else
static char* winTitle;
#endif

static size_t screenWidth;
static size_t screenHeight;
static size_t gameWidth;
static size_t gameHeight;

static bool keysPressed[256] = { false };

LRESULT CALLBACK WinProc(HWND winHWND, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
    //Create the window registration info
    WNDCLASSEX wcex { };

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WinProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(wcex.hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(0, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = 0;
    wcex.lpszClassName = WINDOW_CLASS_NAME;
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    //Try to register the window
    if (!RegisterClassEx(&wcex)) {
        MessageBox(0, _T("Call to RegisterClassEx failed!"), _T("Error"), 0);

        return 1;
    }

    hInst = hInstance;

    //Set the resolution variables
    screenWidth = (size_t)GetSystemMetrics(SM_CXSCREEN);
    screenHeight = (size_t)GetSystemMetrics(SM_CYSCREEN);
    gameWidth = screenWidth;
    gameHeight = screenHeight;

#ifdef UNICODE
    winTitle = new wchar_t[wcslen(L"Unnamed Game") + 1];
    wcscpy_s(winTitle, wcslen(L"Unnamed Game") + 1, L"Unnamed Game");
#else
    winTitle = new char[strlen("Unnamed Game") + 1];
    strcpy(winTitle, "Unnamed Game");
#endif
    
    //Create the HWND
    windowHWND = CreateWindowExW(
        0,
        WINDOW_CLASS_NAME,
        (const TCHAR*)winTitle,
        WS_POPUP,
        0, 0,
        (int)screenWidth, (int)screenHeight,
        0,
        0,
        hInstance,
        0
    );

    if (!windowHWND) {
        MessageBox(0, _T("Call to CreateWindow failed!"), _T("Error"), 0);

        return 1;
    }
    
    ShowWindow(windowHWND, nCmdShow);
    UpdateWindow(windowHWND);

    //Main message loop
    bool gameRunning = true;
    MSG message;
    while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
        if (message.message == WM_QUIT)
            gameRunning = false;

        TranslateMessage(&message);
        DispatchMessage(&message);
    }
    
    return (int)message.wParam;
}

LRESULT CALLBACK WinProc(HWND winHWND, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE: 
        windowHWND = winHWND;

        mge::InitiatePipeline();
        mge::OSMOnStart();
        break;
    case WM_PAINT: 
    {
        //Check for every key if it was just pressed or released
        for (int i = 0; i < 256; i++) {
            bool currentKeyPressed = GetAsyncKeyState(i) >> 15;

            if (!keysPressed[i] && currentKeyPressed)
                mge::OSMOnButtonDown(i);
            else if (keysPressed[i] && !currentKeyPressed)
                mge::OSMOnButtonUp(i);

            keysPressed[i] = currentKeyPressed;
        }
        
        //Set the cursor position variable
        POINT mousePoint;
        GetCursorPos(&mousePoint);
        mge::OSMSetMousePos((size_t)mousePoint.x, (size_t)mousePoint.y);

        mge::OSMOnUpdate();
        break;
    }
    case WM_CLOSE:
        mge::ClearPipeline();
        DestroyWindow(winHWND);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(winHWND, message, wParam, lParam);
        break;
    }
    
    return 0;
}

HWND mge::WindowsGetWindowHWND() {
    return windowHWND;
}
HINSTANCE mge::WindowsGetWindowHInstance() {
    return hInst;
}

#pragma endregion

#pragma region External Functions

size_t mge::WindowsGetScreenWidth() {
    return screenWidth;
}
size_t mge::WindowsGetScreenHeight() {
    return screenHeight;
}
size_t mge::WindowsGetGameWidth() {
    return gameWidth;
}
size_t mge::WindowsGetGameHeight() {
    return gameHeight;
}

char* mge::WindowsGetTitle() {
#ifdef UNICODE
    const size_t tSize = wcslen(winTitle) + 1;
    char* charWinTitle = new char[tSize];
    size_t tConvCount;

    wcstombs_s(&tConvCount, charWinTitle, tSize, winTitle, tSize);

    if (tConvCount != tSize)
        throw std::runtime_error("Not enough characters converted!");

    return charWinTitle;
#else
    return winTitle;
#endif
}
void mge::WindowsSetTitle(const char* newTitle) {
#ifdef UNICODE
    delete[] winTitle;

    const size_t TSize = strlen(newTitle) + 1;
    winTitle = new wchar_t[TSize];
    size_t TConvCount;

    mbstowcs_s(&TConvCount, winTitle, TSize, newTitle, TSize);

    if (TConvCount != TSize)
        throw std::runtime_error("Not enough characters converted!");

    SetWindowText(windowHWND, winTitle);
#else
    winTitle = new char[strlen(newTitle) + 1];
    strcpy(winTitle, newTitle);

    SetWindowText(WindowHWND, WinTitle);
#endif
}

void mge::WindowsSetGameSize(size_t newWidth, size_t newHeight) {
    gameWidth = newWidth;
    gameHeight = newHeight;
}

#pragma endregion
