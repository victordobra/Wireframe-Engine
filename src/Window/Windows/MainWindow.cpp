#include "BuildInfo.hpp"

#ifdef PLATFORM_WINDOWS

/*
    ⚠️ LIVE FINGER REACTION ⚠️
⠀⠀⠀⠀⠀⠀⠀⣀⣴⣶⣿⣿⣿⣿⣿⣿⣿⣶⣦⣀⠀⠀⠀⠀⠀⠀   
      ⣤⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣄⠀⠀⠀   
⠀⠀⠀⢀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧⠀⠀⠀  
   ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣟⣛⣻⣿⣿⣟⣿⣿⣿⣷⠀⠀  
   ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣫⣽⣾⣻⣾⣿⣿⣿⣿⡿⣿⣿⠀⠀  
  ⢰⣿⣿⣻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠻⡿⠿⠟⠛⣟⣿⣽⠀⠀  
 ⠀⠸⣿⣿⣿⣷⣿⣿⣿⣿⡿⠍⠈⠀⠁⣴⡆⠀⠀⠠⢭⣮⣿⡶⠀
⡴⠲⣦⢽⣿⣿⣿⣿⣿⣟⣩⣨⣀⡄⣐⣾⣿⣿⣇⠠⣷⣶⣿⣿⡠⠁
⠃⢀⡄⠀⢻⣿⣿⣿⣿⣽⢿⣿⣯⣾⣿⣿⣿⣿⣿⢿⣿⣿⡟⣿⠀⠀
 ⠣⠧⠀⢿⣿⣿⣿⣿⣿⣿⣿⣿⠟⢸⣿⠿⠿⠿⣧⠙⣿⣿⡿⠀⠀
   ⠁⠼⣒⡿⣿⣿⣿⣿⣿⣿⣿⣠⣬⠀⠀⠀⠀⣾⣷⡈⣿⡇⠀
     ⠉⢳⣿⣿⣿⣿⣿⣿⣿⢟⠗⠼⠖⠒⠔⠉⠉⠻⣿⠇⠀
       ⠈⣻⡿⣿⣿⣿⣿⡿⡀⣤⡄⠸⣰⣾⡒⣷⣴⣿⠀
       ⠂⢸⡗⡄⠘⠭⣭⣷⣿⣮⣠⣌⣫⣿⣷⣿⣿⠃⠀
       ⠈⠀⢸⣿⣾⣷⣦⡿⣿⣿⣿⡿⢻⠞⣹⣿⣿⠏⠀
       ⢘⠀⠘⢻⡿⢿⣋⣤⣤⠌⠉⠛⠛⠀⠈⠉⠁⠀⠀
*/

#include "MainWindow.hpp"
#include "Vulkan/Device.hpp"
#include "Vulkan/SwapChain.hpp"
#include "Vulkan/Sampler.hpp"
#include "Systems/GameLoop.hpp"
#include "Base/Node.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>

// Const variables
const mge::char_t* className = "Application";
const mge::char_t* appName = "MG Engine";

// Variables
HWND hWnd;
HINSTANCE hInstance;
mge::size_t screenWidth, screenHeight;

// WndProc predeclaration
LRESULT CALLBACK WinProc(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam);

// Internal helper functions
static void RegisterApplicationClass() {
    // Create the class info
    WNDCLASSEX wcex{0};

    wcex.cbSize        = sizeof(WNDCLASSEX);
    wcex.style         = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc   = WinProc;
    wcex.cbClsExtra    = 0;
    wcex.cbWndExtra    = 0;
    wcex.hInstance     = hInstance;
    wcex.hIcon         = LoadIcon(wcex.hInstance, IDI_APPLICATION);
    wcex.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wcex.lpszMenuName  = NULL;
    wcex.lpszClassName = className;
    wcex.hIconSm       = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    // Throw an error if the class wasn't registered
    if (!RegisterClassEx(&wcex))
        mge::console::OutFatalError("Failed to register class!", 1);
}
static void CreateHWnd() {
    // Set the screen width and height
    screenWidth = (mge::size_t)GetSystemMetrics(SM_CXSCREEN);
    screenHeight = (mge::size_t)GetSystemMetrics(SM_CYSCREEN);

    // Create the window
    hWnd = CreateWindowEx(
        0,
        className,
        appName,
        WS_POPUP,
        0, 0,
        screenWidth, screenHeight,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    // Throw an error if the window wasn't created
    if (!hWnd)
        mge::console::OutFatalError("Failed to create window!", 1);

    // Show and update the window
    ShowWindow(hWnd, SW_NORMAL);
    UpdateWindow(hWnd);
}

// Windows functions
int WINAPI WinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE hPrevInst, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
    hInstance = hInst;

    mge::console::OpenLogFile();

    RegisterApplicationClass();
    CreateHWnd();

    // Main message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}
LRESULT CALLBACK WinProc(_In_ HWND hWindow, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam) {
    switch(message) {
    case WM_CREATE: 
    {
        hWnd = hWindow;
        mge::CreateVulkanDevice();
        mge::CreateSwapChain({ (mge::uint32_t)screenWidth, (mge::uint32_t)screenHeight });
        mge::CreateSampler();

        mge::Asset::LoadAllAssets();
        mge::Node::SortNodeTypes();
        mge::Node::scene = new mge::Node();
        mge::StartGameLoop();
    }
        return 0;
    case WM_PAINT: 
        mge::UpdateGameLoop();
        return 0;
    case WM_CLOSE:
        delete mge::Node::scene;
        mge::Asset::DeleteAllAssets();

        mge::DeleteSampler();
        mge::DeleteSwapChain();
        mge::DeleteVulkanDevice();
        
        mge::console::CloseLogFile();
        DestroyWindow(hWindow);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hWindow, message, wParam, lParam);
    }

    return 0;
}

// External functions
mge::size_t mge::GetWidth() {
    return screenWidth;
}
mge::size_t mge::GetHeight() {
    return screenHeight;
}
const mge::char_t* mge::GetName() {
    return appName;
}
void mge::SetName(const mge::char_t* newName) {
    appName = newName;
    SetWindowText(hWnd, appName);
}
mge::WindowHandle mge::GetHandle() {
    return hWnd;
}
HINSTANCE mge::GetWindowsInstance() {
    return hInstance;
}

#endif