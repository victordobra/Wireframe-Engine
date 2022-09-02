#include "BuildInfo.hpp"

#ifdef PLATFORM_WINDOWS

#include "MainWindow.hpp"
#include "Vulkan/Device.hpp"
#include "Vulkan/SwapChain.hpp"
#include "Vulkan/Sampler.hpp"
#include "General/GameLoop.hpp"
#include "Base/Asset.hpp"
#include "ECS/EngineECS.hpp"
#include "ProjectInfo.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>

// Variables
wfe::string className = "Application";
wfe::string appName = PROJECT_NAME;

HWND hWnd;
HINSTANCE hInstance;
wfe::size_t screenWidth, screenHeight;

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
    wcex.lpszClassName = className.c_str();
    wcex.hIconSm       = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    // Throw an error if the class wasn't registered
    if (!RegisterClassEx(&wcex))
        wfe::console::OutFatalError("Failed to register class!", 1);
}
static void CreateHWnd() {
    // Set the screen width and height
    screenWidth = (wfe::size_t)GetSystemMetrics(SM_CXSCREEN);
    screenHeight = (wfe::size_t)GetSystemMetrics(SM_CYSCREEN);

    // Create the window
    hWnd = CreateWindowEx(
        0,
        className.c_str(),
        appName.c_str(),
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
        wfe::console::OutFatalError("Failed to create window!", 1);

    // Show and update the window
    ShowWindow(hWnd, SW_NORMAL);
    UpdateWindow(hWnd);
}

// Windows functions
int main(int argc, char** args) {
    hInstance = GetModuleHandle(NULL);

    for(int32_t i = 0; i < argc; ++i) {
        if(!strcmp(args[i], "--vkdebug"))
            wfe::EnableValidationLayers();
        else if(!strcmp(args[i], "--novkdebug"))
            wfe::DisableValidationLayers();
    }

    wfe::console::OpenLogFile();

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
        wfe::Component::AddAssetTypeProperties();
        wfe::CreateDevice();
        wfe::CreateSwapChain();
        wfe::CreateSampler();
        
        wfe::StartGameLoop();
    }
        return 0;
    case WM_PAINT: 
        wfe::UpdateGameLoop();
        return 0;
    case WM_CLOSE:
        delete wfe::GameObject::scene;
        wfe::Asset::DeleteAllAssets();

        wfe::DeleteSampler();
        wfe::DeleteSwapChain();
        wfe::DeleteDevice();
        
        wfe::console::CloseLogFile();
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
wfe::size_t wfe::GetMainWindowWidth() {
    return screenWidth;
}
wfe::size_t wfe::GetMainWindowHeight() {
    return screenHeight;
}
wfe::string wfe::GetMainWindowName() {
    return appName;
}
void wfe::SetMainWindowName(const wfe::string& newName) {
    appName = newName;
    SetWindowText(hWnd, appName.c_str());
}
HWND wfe::GetMainWindowHandle() {
    return hWnd;
}
HINSTANCE wfe::GetWindowsInstance() {
    return hInstance;
}

#endif