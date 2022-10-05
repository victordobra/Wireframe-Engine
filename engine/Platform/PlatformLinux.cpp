#include "BuildInfo.hpp"

#ifdef PLATFORM_LINUX

#include "Platform.hpp"
#include "General/Application.hpp"
#include "ProjectInfo.hpp"
#include <X11/keysym.h>

// Variables
wfe::PlatformInfo platformInfo;

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
static wfe::KeyCode XMouseButtonToKeyCode(wfe::uint32_t mouseButton) {
    switch(mouseButton) {
    case 1:
        return wfe::KEY_LBUTTON;
    case 2:
        return wfe::KEY_MBUTTON;
    case 3:
        return wfe::KEY_RBUTTON;
    default:
        return wfe::KEY_NONE;
    }
}

static void SetFullscreen(wfe::bool8_t fullscreen) {
    XEvent fullscreenEvent;
    
    fullscreenEvent.type = ClientMessage;
    fullscreenEvent.xclient.window = platformInfo.window;
    fullscreenEvent.xclient.message_type = platformInfo.windowManagerState;
    fullscreenEvent.xclient.format = 32;
    fullscreenEvent.xclient.data.l[0] = fullscreen;
    fullscreenEvent.xclient.data.l[1] = platformInfo.fullscreenMessageAtom;
    fullscreenEvent.xclient.data.l[2] = 0;

    XSendEvent(platformInfo.display, platformInfo.screen->root, false, SubstructureNotifyMask | SubstructureRedirectMask, &fullscreenEvent);
}

static void ConnectToX() {
    // Open the display
    platformInfo.display = XOpenDisplay(NULL);
    if(!platformInfo.display)
        wfe::console::OutFatalError("Failed to find display!", 1);

    // Get the default screen
    platformInfo.screenIndex = DefaultScreen(platformInfo.display);

    platformInfo.screen = XScreenOfDisplay(platformInfo.display, platformInfo.screenIndex);
}
static void CreateWindow() {
    // Create the window
    platformInfo.window = XCreateSimpleWindow(platformInfo.display, platformInfo.screen->root, 0, 0, platformInfo.screen->width, platformInfo.screen->height, 0, platformInfo.screen->black_pixel, platformInfo.screen->black_pixel);

    // Select the event masks
    XSelectInput(platformInfo.display, platformInfo.window, KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | StructureNotifyMask);

    // Set the window's name
    XStoreName(platformInfo.display, platformInfo.window, WFE_PROJECT_NAME);

    // Set the window closing atom
    platformInfo.deleteMessageAtom = XInternAtom(platformInfo.display, "WM_DELETE_WINDOW", false);
    XSetWMProtocols(platformInfo.display, platformInfo.window, &platformInfo.deleteMessageAtom, 1);

    // Map the window
    XMapWindow(platformInfo.display, platformInfo.window);

    // Set the fullscreen message atom
    platformInfo.windowManagerState = XInternAtom(platformInfo.display, "_NET_WM_STATE", false);
    platformInfo.fullscreenMessageAtom = XInternAtom(platformInfo.display, "_NET_WM_STATE_FULLSCREEN", false);

    // Wait for the map notify event
    XEvent event;
    while(event.type != MapNotify)
        XNextEvent(platformInfo.display, &event);

    // Flush the display
    XFlush(platformInfo.display);
}
static void ProcessEvent(const XEvent& event) {
    switch(event.type) {
    case KeyPress:
    {
        wfe::KeyCode keyCode = KeySymToKeyCode(event.xkey.keycode);

        // Create the key event
        wfe::KeyEventInfo* keyEventInfo = new wfe::KeyEventInfo();
        keyEventInfo->pressed = true;
        keyEventInfo->keyCode = keyCode;

        wfe::Event keyEvent;
        keyEvent.eventType = wfe::EVENT_TYPE_KEY;
        keyEvent.eventInfo = keyEventInfo;

        // Add the key event to the queue
        wfe::AddEvent(keyEvent);

        break;
    }
    case KeyRelease:
    {
        wfe::KeyCode keyCode = KeySymToKeyCode(event.xkey.keycode);

        // Create the key event
        wfe::KeyEventInfo* keyEventInfo = new wfe::KeyEventInfo();
        keyEventInfo->pressed = false;
        keyEventInfo->keyCode = keyCode;

        wfe::Event keyEvent;
        keyEvent.eventType = wfe::EVENT_TYPE_KEY;
        keyEvent.eventInfo = keyEventInfo;

        // Add the key event to the queue
        wfe::AddEvent(keyEvent);

        break;
    }
    case ButtonPress:
    {
        wfe::KeyCode keyCode = XMouseButtonToKeyCode(event.xbutton.button);

        // Create the key event
        wfe::KeyEventInfo* keyEventInfo = new wfe::KeyEventInfo();
        keyEventInfo->pressed = true;
        keyEventInfo->keyCode = keyCode;

        wfe::Event keyEvent;
        keyEvent.eventType = wfe::EVENT_TYPE_KEY;
        keyEvent.eventInfo = keyEventInfo;

        // Add the key event to the queue
        wfe::AddEvent(keyEvent);

        break;
    }
    case ButtonRelease:
    {
        wfe::KeyCode keyCode = XMouseButtonToKeyCode(event.xbutton.button);

        // Create the key event
        wfe::KeyEventInfo* keyEventInfo = new wfe::KeyEventInfo();
        keyEventInfo->pressed = false;
        keyEventInfo->keyCode = keyCode;

        wfe::Event keyEvent;
        keyEvent.eventType = wfe::EVENT_TYPE_KEY;
        keyEvent.eventInfo = keyEventInfo;

        // Add the key event to the queue
        wfe::AddEvent(keyEvent);

        break;
    }
    case MotionNotify: 
    {
        // Create the mouse move event
        wfe::MouseMoveEventInfo* mouseMoveEventInfo = new wfe::MouseMoveEventInfo();
        mouseMoveEventInfo->mouseX = event.xmotion.x;
        mouseMoveEventInfo->mouseY = event.xmotion.y;

        wfe::Event mouseMoveEvent;
        mouseMoveEvent.eventType = wfe::EVENT_TYPE_MOUSE_MOVE;
        mouseMoveEvent.eventInfo = mouseMoveEventInfo;

        // Add the mouse move event to the queue
        wfe::AddEvent(mouseMoveEvent);

        break;
    }
    case ConfigureNotify:
    {
        // Create the window move event
        wfe::WindowMoveEventInfo* windowMoveEventInfo = new wfe::WindowMoveEventInfo();
        windowMoveEventInfo->windowX = event.xconfigure.x;
        windowMoveEventInfo->windowY = event.xconfigure.y;

        wfe::Event windowMoveEvent;
        windowMoveEvent.eventType = wfe::EVENT_TYPE_WINDOW_MOVE;
        windowMoveEvent.eventInfo = windowMoveEventInfo;

        // Add the window move event to the queue
        wfe::AddEvent(windowMoveEvent);

        // Create the window resize event
        wfe::WindowResizeEventInfo* windowResizeEventInfo = new wfe::WindowResizeEventInfo();
        windowResizeEventInfo->windowWidth = event.xconfigure.width;
        windowResizeEventInfo->windowHeight = event.xconfigure.height;

        wfe::Event windowResizeEvent;
        windowResizeEvent.eventType = wfe::EVENT_TYPE_WINDOW_RESIZE;
        windowResizeEvent.eventInfo = windowResizeEventInfo;

        // Add the window resize event to the queue
        wfe::AddEvent(windowResizeEvent);

        break;
    }
    case ClientMessage:
        if(event.xclient.data.l[0] == platformInfo.deleteMessageAtom) {
            // Exit the application
            wfe::CloseApplication(0);
        }
        break;
    }
}

// Public functions
void wfe::CreatePlatform() {
    ConnectToX();
    CreateWindow();
}
void wfe::DeletePlatform() {
    // Nothing required to delete the platform
}
void wfe::PollPlatformEvents() {
    // Process every pending event from X
    XEvent xEvent;
    while(XPending(platformInfo.display)) {
        // Get the next event
        XNextEvent(platformInfo.display, &xEvent);

        // Process the event
        ProcessEvent(xEvent);
    }

    // Get every window rename event
    auto renameEvents = wfe::GetEventsOfType(wfe::EVENT_TYPE_WINDOW_RENAME);

    // Chech if any window rename events exist
    if(renameEvents.size()) {
        // Set the window's name to the last rename event
        wfe::WindowRenameEventInfo* windowRenameEventInfo = (wfe::WindowRenameEventInfo*)renameEvents.back().eventInfo;

        XStoreName(platformInfo.display, platformInfo.window, windowRenameEventInfo->newName.c_str());
    }

    // Get every window fullscreen event
    auto fullscreenEvents = wfe::GetEventsOfType(wfe::EVENT_TYPE_WINDOW_FULLSCREEN);

    // Check if any window fullscreen events exist
    if(fullscreenEvents.size()) {
        // Set whether the window should be fullscreen based on the last event
        wfe::WindowFullscreenEventInfo* windowFullscreenEventInfo = (wfe::WindowFullscreenEventInfo*)fullscreenEvents.back().eventInfo;

        SetFullscreen(windowFullscreenEventInfo->enabled);
    }
}

wfe::PlatformInfo* wfe::GetPlatformInfo() {
    return &platformInfo;
}

VkResult wfe::CreatePlatformSurface(VkInstance instance, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) {
    // Create the Xlib surface create info
    VkXlibSurfaceCreateInfoKHR createInfo;

    createInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
    createInfo.pNext = nullptr;
    createInfo.flags = 0;
    createInfo.dpy = platformInfo.display;
    createInfo.window = platformInfo.window;

    // Create the Xlib surface
    return vkCreateXlibSurfaceKHR(instance, &createInfo, pAllocator, pSurface);
}

#endif