#include <iostream>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

Display* xDisplay;
int xScreenNum = 0;
Screen* xScreen;
Window xWindow, xRoot;
XEvent xe;

Atom wmDelete;

bool isOpen = true;
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

    wmDelete = XInternAtom(xDisplay, "WM_DELETE_WINDOW", true);
    XSetWMProtocols(xDisplay, xWindow, &wmDelete, 1);

    XMapWindow(xDisplay, xWindow);
    XGetGeometry(xDisplay, xWindow, &xRoot, &position[0], &position[1], &size[0], &size[1], &borderWidth, &depth);

    while (isOpen)
    {
        while (XPending(xDisplay))
        {
            XNextEvent(xDisplay, &xe);
            switch (xe.type)
            {
                case ConfigureNotify:
                    position[0] = xe.xconfigure.x;
                    position[1] = xe.xconfigure.y;
                    size[0] = xe.xconfigure.width;
                    size[1] = xe.xconfigure.height;
                    break;
                case ClientMessage:
                    if (xe.xclient.data.l[0] == static_cast<long int>(wmDelete))
                    {
                        isOpen = false;
                    }
                    break;
            }
        }
    }

    XDestroyWindow(xDisplay, xWindow);
    XCloseDisplay(xDisplay);

    return 0;
}
