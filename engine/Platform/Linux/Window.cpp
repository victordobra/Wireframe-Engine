#include "BuildInfo.hpp"

#ifdef WFE_PLATFORM_LINUX

#include "Platform/Window.hpp"
#include "Core.hpp"
#include "General/Engine.hpp"
#include "ProjectInfo.hpp"

#include <X11/Xlib.h>

namespace wfe {
	// Constants
	constexpr int32_t EVENT_MASK = ButtonMotionMask | ButtonPressMask | ButtonReleaseMask | EnterWindowMask | LeaveWindowMask | KeyPressMask | KeyReleaseMask | StructureNotifyMask;
	const char_t* ATOM_NAMES[] = {
		"_NET_WM_STATE",
		"WM_DELETE_WINDOW",
		"_NET_WM_STATE_FULLSCREEN"
	}; 

	// Internal variables
	Display* display;
	int32_t screenIndex;
	Screen* screen;
	Window window;

	Atom windowManagerState;
	Atom deleteMsgAtom;
	Atom fullscreenMsgAtom;

	// Internal helper functions
	static void ConnectToX() {
		// Open the X display
		display = XOpenDisplay(NULL);

		if(!display)
			WFE_LOG_FATAL("Failed to open X display!")
		
		// Get the default screen index
		screenIndex = DefaultScreen(display);

		// Get the screen with the default index
		screen = XScreenOfDisplay(display, screenIndex);
	}
	static void CreateXWindow() {
		// Create the window
		window = XCreateSimpleWindow(display, screen->root, 0, 0, screen->width, screen->height, 0, screen->black_pixel, screen->black_pixel);

		// Set the wanted event mask
		XSelectInput(display, window, EVENT_MASK);

		// Set the window's name
		XStoreName(display, window, WFE_PROJECT_NAME);

		// Map the window
		XMapWindow(display, window);

		// Get all wanted atoms
		Atom internAtoms[3];
		XInternAtoms(display, (char_t**)ATOM_NAMES, 3, false, internAtoms);

		// Set all atoms from the retrieved atom array
		windowManagerState = internAtoms[0];
		deleteMsgAtom = internAtoms[1];
		fullscreenMsgAtom = internAtoms[2];

		// Set the window delete atom
		XSetWMProtocols(display, window, &deleteMsgAtom, 1);

		// Wait for the MapNotify event from the event list and discard any events before it
		XEvent event;
		while(event.type != MapNotify)
			XNextEvent(display, &event);
		
		// Flush the display
		XFlush(display);
	}
	static void ProcessEvent(const XEvent& event) {
		switch(event.type) {
		case ClientMessage:
			// Check if the given message is the delete window message
			if(event.xclient.data.l[0] == deleteMsgAtom) {
				// Close the game with a return code of 0
				wfe::CloseGame(0);
			}
			break;
		}
	}

	// Public functions
	void CreateWindow() {
		ConnectToX();
		CreateXWindow();
	}
	void DeleteWindow() {
		// No action required to delete the window
	}
	void PollWindowEvents() {
		// Process every pending event from X
		int32_t eventCount = XPending(display);

		XEvent event;
		for(int32_t i = 0; i != eventCount; ++i) {
			// Get the next event
			XNextEvent(display, &event);

			// Process the event
			ProcessEvent(event);
		}
	}
}

#endif