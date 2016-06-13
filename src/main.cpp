#include <stdio.h>
#include <stdlib.h>

#include <allegro5/allegro.h>

#include "MCP.h"

int main (int argc, char **argv)
    {
    if ( !al_init() )
        {
        fprintf(stderr, "3df: failed to initialize allegro\n");
        return -1;
        }

    // TODO  parse command arguments

    if ( mcp::Initialize() )  // TODO  pass init commands here
        {
        mcp::MainLoop();
        }

char buf[501];
fgets(buf, 500, stdin);
    return 0;
    }

