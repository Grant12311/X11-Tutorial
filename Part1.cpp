#include <iostream>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

Display* xDisplay;
int xScreenNum = 0;
Screen* xScreen;
Window xWindow, xRoot;

int position[2];
unsigned int size[2];
unsigned int borderWidth;
unsigned int depth;

int main()
{
    xDisplay = XOpenDisplay(NULL);
    if (xDisplay == NULL)
    {
        std::cerr << "Error! Cannot open X11 display" << std::endl;
        return 1;
    }

    xScreenNum = XDefaultScreen(xDisplay);
    xScreen = XDefaultScreenOfDisplay(xDisplay);

    xWindow = XCreateSimpleWindow(xDisplay,                          // Target Display
                                  XRootWindow(xDisplay, xScreenNum), // Parent Window
                                  10,                                 // Position X
                                  10,                                // Position Y
                                  200,                               // Width
                                  300,                               // Height
                                  1,                                 // Border Width
                                  BlackPixel(xDisplay, xScreenNum),  // Border Color
                                  WhitePixel(xDisplay, xScreenNum)); // Background Color

    XSetStandardProperties(xDisplay, xWindow, "I'm learning X11", "This is the icon name", None, NULL, 0, NULL);
    XSelectInput(xDisplay, xWindow, StructureNotifyMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask);

    XMapWindow(xDisplay, xWindow);
    XGetGeometry(xDisplay, xWindow, &xRoot, &position[0], &position[1], &size[0], &size[1], &borderWidth, &depth);

    while (true) {}

    XDestroyWindow(xDisplay, xWindow);
    XCloseDisplay(xDisplay);

    return 0;
}
