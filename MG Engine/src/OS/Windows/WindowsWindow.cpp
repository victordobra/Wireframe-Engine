#include "Defines.h"

#include "Windows.h"
#include "WindowsInternal.h"

#include "GameLoopManager.h"
#include "RenderingPipeline.h"
#include "DebuggerInternal.h"
#include "Debugger.h"

#include <windows.h>
#include <tchar.h>

#define WINDOW_CLASS_NAME _T("Desktop App")

//Windows variables
static HINSTANCE hInst;
static HDC windowHDC;
static HWND windowHWND;

//Title variable
#ifdef UNICODE
static mge::charw_t* winTitle;
#else
static mge::char_t* winTitle;
#endif

//Screen size
static mge::size_t screenWidth;
static mge::size_t screenHeight;

//Thread variables
HANDLE hThread;

//Other variables
static mge::bool8_t windowActive = true;

LRESULT CALLBACK WinProc(HWND winHWND, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE:
        windowHWND = winHWND;

        mge::InitiateDebugger();
        mge::InitiatePipeline();
        mge::StartGameLoop();
        break;
    case WM_PAINT:
        mge::UpdateGameLoop();
        break;
    case WM_CLOSE:
        DestroyWindow(winHWND);
        break;
    case WM_DESTROY:
        mge::EndGameLoop();
        mge::ClearPipeline();
        mge::ClearDebugger();
        PostQuitMessage(0);
        break;
    case WM_ACTIVATE:
        windowActive = wParam != WA_INACTIVE;
        return DefWindowProc(winHWND, message, wParam, lParam);
    default:
        return DefWindowProc(winHWND, message, wParam, lParam);
    }

    return 0;
}
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
    //Create the window registration info
    WNDCLASSEX wcex{};

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
    screenWidth = (mge::size_t)GetSystemMetrics(SM_CXSCREEN);
    screenHeight = (mge::size_t)GetSystemMetrics(SM_CYSCREEN);

#ifdef UNICODE
    winTitle = new mge::charw_t[wcslen(L"Unnamed Game") + 1];
    wcscpy_s(winTitle, wcslen(L"Unnamed Game") + 1, L"Unnamed Game");
#else
    winTitle = new mge::char_t[strlen("Unnamed Game") + 1];
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
    MSG message;
    while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
    
    return (int)message.wParam;
}

//Internal functions
HWND mge::WindowsGetWindowHWND() {
    return windowHWND;
}
HINSTANCE mge::WindowsGetWindowHInstance() {
    return hInst;
}

//External functions
mge::size_t mge::WindowsGetScreenWidth() {
    return screenWidth;
}
mge::size_t mge::WindowsGetScreenHeight() {
    return screenHeight;
}
mge::bool8_t mge::WindowsIsWindowActive() {
    return windowActive;
}

mge::char_t* mge::WindowsGetTitle() {
#ifdef UNICODE
    const mge::size_t tSize = wcslen(winTitle) + 1;
    char_t* charWinTitle = new char_t[tSize];
    mge::size_t tConvCount;

    wcstombs_s(&tConvCount, charWinTitle, tSize, winTitle, tSize);

    if (tConvCount != tSize)
        OutFatalError("Not enough characters converted!");

    return charWinTitle;
#else
    return winTitle;
#endif
}
void mge::WindowsSetTitle(const char_t* newTitle) {
#ifdef UNICODE
    delete[] winTitle;

    const mge::size_t TSize = strlen(newTitle) + 1;
    winTitle = new mge::charw_t[TSize];
    mge::size_t TConvCount;

    mbstowcs_s(&TConvCount, winTitle, TSize, newTitle, TSize);

    if (TConvCount != TSize)
        OutFatalError("Not enough characters converted!");

    SetWindowText(windowHWND, winTitle);
#else
    winTitle = new mge::char_t[strlen(newTitle) + 1];
    strcpy(winTitle, newTitle);

    SetWindowText(windowHWND, winTitle);
#endif
}
