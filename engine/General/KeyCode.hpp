#pragma once

#include "Core.hpp"

// KEY_EXECUTE define safeguard
#ifdef KEY_EXECUTE
#undef KEY_EXECUTE
#endif

namespace wfe {
    typedef enum : uint8_t {
        KEY_LBUTTON = 0x01,
        KEY_RBUTTON = 0x02,
        KEY_CANCEL = 0x03,
        KEY_MBUTTON = 0x04,
        KEY_X_BUTTON1 = 0x05,
        KEY_X_BUTTON2 = 0x06,
        KEY_BACKSPACE = 0x08,
        KEY_TAB = 0x09,
        KEY_CLEAR = 0x0C,
        KEY_ENTER = 0x0D,
        KEY_SHIFT = 0x10,
        KEY_CONTROL = 0x11,
        KEY_MENU = 0x12,
        KEY_PAUSE = 0x13,
        KEY_CAPS_LOCK = 0x14,
        KEY_KANA = 0x15,
        KEY_HANGUEL = 0x15,
        KEY_HANGUL = 0x15,
        KEY_IME_ON = 0x16,
        KEY_JUNJA = 0x17,
        KEY_FINAL = 0x18,
        KEY_HANJA = 0x19,
        KEY_KANJI = 0x19,
        KEY_IME_OFF = 0x1A,
        KEY_ESCAPE = 0x1B,
        KEY_CONVERT = 0x1C,
        KEY_NONCONVERT = 0x1D,
        KEY_ACCEPT = 0x1E,
        KEY_MODECHANGE = 0x1F,
        KEY_SPACE = 0x20,
        KEY_PRIOR = 0x21,
        KEY_NEXT = 0x22,
        KEY_END = 0x23,
        KEY_HOME = 0x24,
        KEY_LEFT = 0x25,
        KEY_UP = 0x26,
        KEY_RIGHT = 0x27,
        KEY_DOWN = 0x28,
        KEY_SELECT = 0x29,
        KEY_PRINT = 0x2A,
        KEY_EXECUTE = 0x2B,
        KEY_SNAPSHOT = 0x2C,
        KEY_INSERT = 0x2D,
        KEY_KEY_DELETE = 0x2E,
        KEY_HELP = 0x2F,
        KEY_KEY_0 = 0x30,
        KEY_KEY_1 = 0x31,
        KEY_KEY_2 = 0x32,
        KEY_KEY_3 = 0x33,
        KEY_KEY_4 = 0x34,
        KEY_KEY_5 = 0x35,
        KEY_KEY_6 = 0x36,
        KEY_KEY_7 = 0x37,
        KEY_KEY_8 = 0x38,
        KEY_KEY_9 = 0x39,
        KEY_KEY_A = 0x41,
        KEY_KEY_B = 0x42,
        KEY_KEY_C = 0x43,
        KEY_KEY_D = 0x44,
        KEY_KEY_E = 0x45,
        KEY_KEY_F = 0x46,
        KEY_KEY_G = 0x47,
        KEY_KEY_H = 0x48,
        KEY_KEY_I = 0x49,
        KEY_KEY_J = 0x4A,
        KEY_KEY_K = 0x4B,
        KEY_KEY_L = 0x4C,
        KEY_KEY_M = 0x4D,
        KEY_KEY_N = 0x4E,
        KEY_KEY_O = 0x4F,
        KEY_KEY_P = 0x50,
        KEY_KEY_Q = 0x51,
        KEY_KEY_R = 0x52,
        KEY_KEY_S = 0x53,
        KEY_KEY_T = 0x54,
        KEY_KEY_U = 0x55,
        KEY_KEY_V = 0x56,
        KEY_KEY_W = 0x57,
        KEY_KEY_X = 0x58,
        KEY_KEY_Y = 0x59,
        KEY_KEY_Z = 0x5A,
        KEY_LWIN = 0x5B,
        KEY_RWIN = 0x5C,
        KEY_APPS = 0x5D,
        KEY_SLEEP = 0x5F,
        KEY_NUMPAD0 = 0x60,
        KEY_NUMPAD1 = 0x61,
        KEY_NUMPAD2 = 0x62,
        KEY_NUMPAD3 = 0x63,
        KEY_NUMPAD4 = 0x64,
        KEY_NUMPAD5 = 0x65,
        KEY_NUMPAD6 = 0x66,
        KEY_NUMPAD7 = 0x67,
        KEY_NUMPAD8 = 0x68,
        KEY_NUMPAD9 = 0x69,
        KEY_MULTIPLY = 0x6A,
        KEY_ADD = 0x6B,
        KEY_SEPARATOR = 0x6C,
        KEY_SUBTRACT = 0x6D,
        KEY_DECIMAL = 0x6E,
        KEY_DIVIDE = 0x6F,
        KEY_F1 = 0x70,
        KEY_F2 = 0x71,
        KEY_F3 = 0x72,
        KEY_F4 = 0x73,
        KEY_F5 = 0x74,
        KEY_F6 = 0x75,
        KEY_F7 = 0x76,
        KEY_F8 = 0x77,
        KEY_F9 = 0x78,
        KEY_F10 = 0x79,
        KEY_F11 = 0x7A,
        KEY_F12 = 0x7B,
        KEY_F13 = 0x7C,
        KEY_F14 = 0x7D,
        KEY_F15 = 0x7E,
        KEY_F16 = 0x7F,
        KEY_F17 = 0x80,
        KEY_F18 = 0x81,
        KEY_F19 = 0x82,
        KEY_F20 = 0x83,
        KEY_F21 = 0x84,
        KEY_F22 = 0x85,
        KEY_F23 = 0x86,
        KEY_F24 = 0x87,
        KEY_NUM_LOCK	= 0x90,
        KEY_SCROLL_LOCK = 0x91,
        KEY_LSHIFT = 0xA0,
        KEY_RSHIFT = 0xA1,
        KEY_LCONTROL = 0xA2,
        KEY_RCONTROL = 0xA3,
        KEY_LMENU = 0xA4,
        KEY_RMENU = 0xA5,
        KEY_BROWSER_BACK = 0xA6,
        KEY_BROWSER_FORWARD = 0xA7,
        KEY_BROWSER_REFRESH = 0xA8,
        KEY_BROWSER_STOP = 0xA9,
        KEY_BROWSER_SEARCH = 0xAA,
        KEY_BROWSER_FAVORITES = 0xAB,
        KEY_BROWSER_HOME = 0xAC,
        KEY_VOLUME_MUTE = 0xAD,
        KEY_VOLUME_DOWN = 0xAE,
        KEY_VOLUME_UP = 0xAF,
        KEY_MEDIA_NEXT_TRACK = 0xB0,
        KEY_MEDIA_PREV_TRACK = 0xB1,
        KEY_MEDIA_STOP = 0xB2,
        KEY_MEDIA_PLAY_PAUSE = 0xB3,
        KEY_LAUNCH_MAIL = 0xB4,
        KEY_LAUNCH_MEDIA_SELECT = 0xB5,
        KEY_LAUNCH_APP1 = 0xB6,
        KEY_LAUNCH_APP2 = 0xB7,
        KEY_OEM_1 = 0xBA,
        KEY_OEM_PLUS = 0xBB,
        KEY_OEM_COMMA = 0xBC,
        KEY_OEM_MINUS = 0xBD,
        KEY_OEM_PERIOD = 0xBE,
        KEY_OEM_2 = 0xBF,
        KEY_OEM_3 = 0xC0,
        KEY_OEM_4 = 0xDB,
        KEY_OEM_5 = 0xDC,
        KEY_OEM_6 = 0xDD,
        KEY_OEM_7 = 0xDE,
        KEY_OEM_8 = 0xDF,
        KEY_OEM_102	= 0xE2,
        KEY_PROCESSKEY = 0xE5,
        KEY_PACKET = 0xE7,
        KEY_ATTN = 0xF6,
        KEY_CRSEL = 0xF7,
        KEY_EXSEL = 0xF8,
        KEY_EREOF = 0xF9,
        KEY_PLAY = 0xFA,
        KEY_ZOOM = 0xFB,
        KEY_PA1 = 0xFD,
        KEY_OEM_CLEAR = 0xFE
    } KeyCode;
}