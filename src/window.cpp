// Window handler

#include "MCP.h"
#include "Window.h"

bool Window::Init (int x, int y, int width, int height)
    {
    if ( !(display = al_create_display(width, height)) )
       {
fprintf(stderr, "3df: failed to create window!?\n");
       return false;
       }

    al_clear_to_color(al_map_rgb(0,0,0));
    al_flip_display();
    return true;
    }

void Window::Close ()
    {
    al_destroy_display(display);
    }

