#include <Kernel/Window.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <cstring>

namespace
{
	Visual  * visual;
	Display * display;
	Window    window;
	Window    root;
	int32     depth;

	bool running;
	XSetWindowAttributes windowAttrs;

	Atom wmDeleteWindow;
	XIM	im;

	cchar applicationName  = "${title}";
	cchar applicationClass = "HybridAlpaca_Kernel";
}

Kernel::WindowPlatformData Kernel::CreateWindow(cchar title, uint32 width, uint32 height)
{
	applicationName = title;

	// Start the X client in thread compatablility mode
	XInitThreads();

	// Select server display and screen
	display      = XOpenDisplay(0);
	int32 screen = DefaultScreen(display);

	depth  = DefaultDepth  (display, screen);
	visual = DefaultVisual (display, screen);
	root   = RootWindow    (display, screen);

	// Clear window attributes
	memset(& windowAttrs, 0, sizeof(windowAttrs));

	// Set window attributes
	windowAttrs.background_pixmap = 0;
	windowAttrs.border_pixel = 0;
	windowAttrs.event_mask = 0
		| ButtonPressMask      ///< Mouse button clicked
		| ButtonReleaseMask    ///< Mouse button released
		| ExposureMask         ///< Window is visible to user
		| KeyPressMask         ///< Keyboard key pressed
		| KeyReleaseMask       ///< Keyboard key released
		| PointerMotionMask    ///< Mouse pointer is moved
		| StructureNotifyMask; ///< Window state is changed (moved, resized, etc.)

	// Create the window
	window = XCreateWindow
	  ( display       ///< Display
		, root          ///< Parent window
		, 0, 0          ///< XY coords
		, width, height ///< Width height
		, 0             ///< Border width
		, depth         ///< Window depth
		, InputOutput   ///< Window class
		, visual        ///< Visual type

		, CWBorderPixel | CWEventMask ///< Attribute mask
		, & windowAttrs               ///< Window attributes
		);

	XSetWindowAttributes attr;
	memset(& attr, 0, sizeof(attr));
	XChangeWindowAttributes(display, window, CWBackPixel, & attr);

	// Enable window close protocol
	cchar wmDeleteWindowName = "WM_DELETE_WINDOW";
	XInternAtoms(display, (char **) & wmDeleteWindowName, 1, False, & wmDeleteWindow);
	XSetWMProtocols(display, window, & wmDeleteWindow, 1);

	// Map window to display and give it a title
	XMapWindow(display, window);
	XStoreName(display, window, applicationName);

	// Associate our window with a window class
	XClassHint * hint = XAllocClassHint();
	hint -> res_name  = (char *) applicationName;
	hint -> res_class = (char *) applicationClass;
	XSetClassHint(display, window, hint);
	XFree(hint);

	// Open display's input method (IM)
	im = XOpenIM(display, nullptr, nullptr, nullptr);

	running = true;

	ProcessWindowEvents();

	return WindowPlatformData{display, (void *)(uint32) window, nullptr};
}

bool Kernel::WindowOpen()
{
	return running;
}

void Kernel::ProcessWindowEvents()
{
	XEvent event;

	// While there are still events in the display's queue
	while (XPending(display) > 0)
	{
		XNextEvent(display, & event);

		switch(event.type)
		{
			case ClientMessage:
				// [x] button pressed
				if (static_cast<Atom>(event.xclient.data.l[0]) == wmDeleteWindow)
				{
					running = false;
				}
				break;
			case KeyPress:
				// Escape key
				if (event.xkey.keycode == 0x09)
				{
					running = false;
				}
				break;
		}
	}
}

void Kernel::DestroyWindow()
{
	// Destroy input methods and contexts
	XCloseIM(im);

	// Remove and destroy our window
	XUnmapWindow   (display, window);
	XDestroyWindow (display, window);
}
