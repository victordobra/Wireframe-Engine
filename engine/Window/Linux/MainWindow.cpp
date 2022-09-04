#include "BuildInfo.hpp"

#ifdef PLATFORM_LINUX

#include "MainWindow.hpp"
#include "Core.hpp"
#include "Vulkan/Device.hpp"
#include "Vulkan/SwapChain.hpp"
#include "Vulkan/Sampler.hpp"
#include "ECS/EngineECS.hpp"
#include "Base/Asset.hpp"
#include "ProjectInfo.hpp"
#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <X11/keysym.h>

// Variables
Display* display;
wfe::int32_t screenIndex;
Screen* screen;

Window window;
Atom wmDeleteMessage;
wfe::string windowName = PROJECT_NAME;

wfe::bool8_t running = true;

wfe::uint8_t keyDownMap[32];
wfe::MousePos mousePos;

// Internal helper functions
static wfe::KeyCode KeySymToKeyCode(KeySym keySym) {
    switch(keySym) {
    case XK_BackSpace:
        return wfe::KEY_BACKSPACE;
    case XK_Return:
        return wfe::KEY_ENTER;
    case XK_Tab:
        return wfe::KEY_TAB;
    case XK_Pause:
        return wfe::KEY_PAUSE;
    case XK_Caps_Lock:
        return wfe::KEY_CAPS_LOCK;
    case XK_Escape:
        return wfe::KEY_ESCAPE;
    case XK_Mode_switch:
        return wfe::KEY_MODECHANGE;
    case XK_space:
        return wfe::KEY_SPACE;
    case XK_Prior:
        return wfe::KEY_PRIOR;
    case XK_Next:
        return wfe::KEY_NEXT;
    case XK_End:
        return wfe::KEY_END;
    case XK_Home:
        return wfe::KEY_HOME;
    case XK_Left:
        return wfe::KEY_LEFT;
    case XK_Up:
        return wfe::KEY_UP;
    case XK_Right:
        return wfe::KEY_RIGHT;
    case XK_Down:
        return wfe::KEY_DOWN;
    case XK_Select:
        return wfe::KEY_SELECT;
    case XK_Print:
        return wfe::KEY_PRINT;
    case XK_Execute:
        return wfe::KEY_EXECUTE;
    case XK_Insert:
        return wfe::KEY_INSERT;
    case XK_Delete:
        return wfe::KEY_DELETE;
    case XK_Help:
        return wfe::KEY_HELP;
    case XK_Meta_L:
        return wfe::KEY_LWIN;
    case XK_Meta_R:
        return wfe::KEY_RWIN;
    case XK_KP_0:
        return wfe::KEY_NUMPAD0;
    case XK_KP_1:
        return wfe::KEY_NUMPAD1;
    case XK_KP_2:
        return wfe::KEY_NUMPAD2;
    case XK_KP_3:
        return wfe::KEY_NUMPAD3;
    case XK_KP_4:
        return wfe::KEY_NUMPAD4;
    case XK_KP_5:
        return wfe::KEY_NUMPAD5;
    case XK_KP_6:
        return wfe::KEY_NUMPAD6;
    case XK_KP_7:
        return wfe::KEY_NUMPAD7;
    case XK_KP_8:
        return wfe::KEY_NUMPAD8;
    case XK_KP_9:
        return wfe::KEY_NUMPAD9;
    case XK_multiply:
        return wfe::KEY_MULTIPLY;
    case XK_KP_Add:
        return wfe::KEY_ADD;
    case XK_KP_Separator:
        return wfe::KEY_SEPARATOR;
    case XK_KP_Subtract:
        return wfe::KEY_SUBTRACT;
    case XK_KP_Decimal:
        return wfe::KEY_DECIMAL;
    case XK_KP_Divide:
        return wfe::KEY_DIVIDE;
    case XK_F1:
        return wfe::KEY_F1;
    case XK_F2:
        return wfe::KEY_F2;
    case XK_F3:
        return wfe::KEY_F3;
    case XK_F4:
        return wfe::KEY_F4;
    case XK_F5:
        return wfe::KEY_F5;
    case XK_F6:
        return wfe::KEY_F6;
    case XK_F7:
        return wfe::KEY_F7;
    case XK_F8:
        return wfe::KEY_F8;
    case XK_F9:
        return wfe::KEY_F9;
    case XK_F10:
        return wfe::KEY_F10;
    case XK_F11:
        return wfe::KEY_F11;
    case XK_F12:
        return wfe::KEY_F12;
    case XK_F13:
        return wfe::KEY_F13;
    case XK_F14:
        return wfe::KEY_F14;
    case XK_F15:
        return wfe::KEY_F15;
    case XK_F16:
        return wfe::KEY_F16;
    case XK_F17:
        return wfe::KEY_F17;
    case XK_F18:
        return wfe::KEY_F18;
    case XK_F19:
        return wfe::KEY_F19;
    case XK_F20:
        return wfe::KEY_F20;
    case XK_F21:
        return wfe::KEY_F21;
    case XK_F22:
        return wfe::KEY_F22;
    case XK_F23:
        return wfe::KEY_F23;
    case XK_F24:
        return wfe::KEY_F24;
    case XK_Num_Lock:
        return wfe::KEY_NUM_LOCK;
    case XK_Scroll_Lock:
        return wfe::KEY_SCROLL_LOCK;
    case XK_Shift_L:
        return wfe::KEY_LSHIFT;
    case XK_Shift_R:
        return wfe::KEY_RSHIFT;
    case XK_Control_L:
        return wfe::KEY_LCONTROL;
    case XK_Control_R:
        return wfe::KEY_RCONTROL;
    case XK_Alt_L:
        return wfe::KEY_LALT;
    case XK_Alt_R:
        return wfe::KEY_RALT;
    case XK_semicolon:
        return wfe::KEY_OEM_1;
    case XK_plus:
        return wfe::KEY_PLUS;
    case XK_comma:
        return wfe::KEY_COMMA;
    case XK_minus:
        return wfe::KEY_MINUS;
    case XK_period:
        return wfe::KEY_PERIOD;
    case XK_slash:
        return wfe::KEY_OEM_2;
    case XK_grave:
        return wfe::KEY_OEM_3;
    case XK_0:
        return wfe::KEY_0;
    case XK_1:
        return wfe::KEY_1;
    case XK_2:
        return wfe::KEY_2;
    case XK_3:
        return wfe::KEY_3;
    case XK_4:
        return wfe::KEY_4;
    case XK_5:
        return wfe::KEY_5;
    case XK_6:
        return wfe::KEY_6;
    case XK_7:
        return wfe::KEY_7;
    case XK_8:
        return wfe::KEY_8;
    case XK_9:
        return wfe::KEY_9;
    case XK_a:
    case XK_A:
        return wfe::KEY_A;
    case XK_b:
    case XK_B:
        return wfe::KEY_B;
    case XK_c:
    case XK_C:
        return wfe::KEY_C;
    case XK_d:
    case XK_D:
        return wfe::KEY_D;
    case XK_e:
    case XK_E:
        return wfe::KEY_E;
    case XK_f:
    case XK_F:
        return wfe::KEY_F;
    case XK_g:
    case XK_G:
        return wfe::KEY_G;
    case XK_h:
    case XK_H:
        return wfe::KEY_H;
    case XK_i:
    case XK_I:
        return wfe::KEY_I;
    case XK_j:
    case XK_J:
        return wfe::KEY_J;
    case XK_k:
    case XK_K:
        return wfe::KEY_K;
    case XK_l:
    case XK_L:
        return wfe::KEY_L;
    case XK_m:
    case XK_M:
        return wfe::KEY_M;
    case XK_n:
    case XK_N:
        return wfe::KEY_N;
    case XK_o:
    case XK_O:
        return wfe::KEY_O;
    case XK_p:
    case XK_P:
        return wfe::KEY_P;
    case XK_q:
    case XK_Q:
        return wfe::KEY_Q;
    case XK_r:
    case XK_R:
        return wfe::KEY_R;
    case XK_s:
    case XK_S:
        return wfe::KEY_S;
    case XK_t:
    case XK_T:
        return wfe::KEY_T;
    case XK_u:
    case XK_U:
        return wfe::KEY_U;
    case XK_v:
    case XK_V:
        return wfe::KEY_V;
    case XK_w:
    case XK_W:
        return wfe::KEY_W;
    case XK_x:
    case XK_X:
        return wfe::KEY_X;
    case XK_y:
    case XK_Y:
        return wfe::KEY_Y;
    case XK_z:
    case XK_Z:
        return wfe::KEY_Z;
    default:
        return wfe::KEY_NONE;
    }
}

static void ConnectToX() {
    // Open the display
    display = XOpenDisplay(NULL);
    if(!display)
        wfe::console::OutFatalError("Failed to find display! Error code: ", 1);
    
    // Disable key repeats
    XAutoRepeatOff(display);

    // Get the default screen
    screenIndex = DefaultScreen(display);

    screen = XScreenOfDisplay(display, screenIndex);
}
static void CreateWindow() {
    // Create the window
    window = XCreateSimpleWindow(display, screen->root, 0, 0, screen->width, screen->height, 0, screen->black_pixel, screen->black_pixel);

    // Select the event masks
    XSelectInput(display, window, KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | ButtonMotionMask | StructureNotifyMask);

    // Set the window's name
    XStoreName(display, window, windowName.c_str());

    // Make the window fullscreen
    Atom wmState = XInternAtom(display, "_NET_WM_STATE", false);
    Atom fullscreen = XInternAtom(display, "_NET_WM_STATE_FULLSCREEN", false);

    XEvent fullscreenEvent;
    
    fullscreenEvent.type = ClientMessage;
    fullscreenEvent.xclient.window = window;
    fullscreenEvent.xclient.message_type = wmState;
    fullscreenEvent.xclient.format = 32;
    fullscreenEvent.xclient.data.l[0] = 1;
    fullscreenEvent.xclient.data.l[1] = fullscreen;
    fullscreenEvent.xclient.data.l[2] = 0;

    // Set the window closing atom
    wmDeleteMessage = XInternAtom(display, "WM_DELETE_WINDOW", false);
    XSetWMProtocols(display, window, &wmDeleteMessage, 1);

    // Map the window
    XMapWindow(display, window);

    // Send the fullscreen event
    XSendEvent(display, screen->root, false, SubstructureNotifyMask | SubstructureRedirectMask, &fullscreenEvent);

    // Wait for the map notify event
    XEvent event;
    while(event.type != MapNotify)
        XNextEvent(display, &event);

    // Flush the display
    XFlush(display);
}
static void ProcessEvent(const XEvent& event) {
    switch(event.type) {
    case KeyPress:
    {
        wfe::KeyCode keyCode = KeySymToKeyCode(event.xkey.keycode);

        keyDownMap[keyCode >> 3] |= 1 << (keyCode & 7);

        break;
    }
    case KeyRelease:
    {
        wfe::KeyCode keyCode = KeySymToKeyCode(event.xkey.keycode);

        keyDownMap[keyCode >> 3] &= ~(1 << (keyCode & 7));

        break;
    }
    case ButtonPress:
    {
        wfe::KeyCode keyCode;
        switch(event.xbutton.button) {
        case 1:
            keyCode = wfe::KEY_LBUTTON;
            break;
        case 2:
            keyCode = wfe::KEY_MBUTTON;
            break;
        case 3:
            keyCode = wfe::KEY_RBUTTON;
            break;
        default:
            keyCode = wfe::KEY_NONE;
            break;
        }

        keyDownMap[keyCode >> 3] |= 1 << (keyCode & 7);

        break;
    }
    case ButtonRelease:
    {
        wfe::KeyCode keyCode;
        switch(event.xbutton.button) {
        case 1:
            keyCode = wfe::KEY_LBUTTON;
            break;
        case 2:
            keyCode = wfe::KEY_MBUTTON;
            break;
        case 3:
            keyCode = wfe::KEY_RBUTTON;
            break;
        default:
            keyCode = wfe::KEY_NONE;
            break;
        }

        keyDownMap[keyCode >> 3] &= ~(1 << (keyCode & 7));

        break;
    }
    case MotionNotify:
        mousePos.x = event.xmotion.x;
        mousePos.y = event.xmotion.y;
        break;
    case ClientMessage:
        if(event.xclient.data.l[0] == wmDeleteMessage) {
            XAutoRepeatOn(display);
            running = false;
        }
        break;
    }
}

int main(int argc, char** args) {
    // Open the log file
    wfe::console::OpenLogFile();

    // Check for any special args
    for(int32_t i = 1; i < argc; ++i) {
        if(!strcmp(args[i], "--vkdebug"))
            wfe::EnableValidationLayers();
        else if(!strcmp(args[i], "--novkdebug"))
            wfe::DisableValidationLayers();
    }

    // Connect to X and create the window
    ConnectToX();
    CreateWindow();

    // Setup the rest of the engine
    wfe::Component::AddAssetTypeProperties();
    
    wfe::CreateDevice();
    wfe::CreateSwapChain();
    wfe::CreateSampler();

    wfe::StartGameLoop();
    wfe::SetMouseState(wfe::MOUSE_STATE_LOCKED);

    XEvent event;
    while(running) {
        // Process every event in the queue
        while(XPending(display)) {
            XNextEvent(display, &event);
            ProcessEvent(event);
        }

        wfe::UpdateGameLoop();
    }

    // Destroy the rest of the engine
    wfe::Asset::DeleteAllAssets();

    wfe::DeleteSampler();
    wfe::DeleteSwapChain();
    wfe::DeleteDevice();

    wfe::console::CloseLogFile();

    return 0;
}

// Public functions
wfe::size_t wfe::GetMainWindowWidth() {
    return screen->width;
}
wfe::size_t wfe::GetMainWindowHeight() {
    return screen->height;
}
wfe::string wfe::GetMainWindowName() {
    return windowName;
}
void SetMainWindowName(const wfe::string& newName) {
    windowName = newName;
    XStoreName(display, window, windowName.c_str());
}

Display* wfe::GetScreenConnection() {
    return display;
}
Window wfe::GetWindowHandle() {
    return window;
}
wfe::bool8_t wfe::IsLinuxKeyDown(wfe::KeyCode keyCode) {
    return (keyDownMap[keyCode >> 3] >> (keyCode & 7)) & 1;
}
wfe::MousePos wfe::GetLinuxMousePos() {
    return mousePos;
}

#endif