// SORRY PURISTS THESE ARE GLOBALS HAHA!!!!!1

// The mcp namespace is considered the global space.
// For the most part items located here do not yet have a proper home.

// Global headers
#include <assert.h>
#include <allegro5/allegro.h>

// Only here for mcp::eventq
#include "Event.h"

class DFState;
class Window;

namespace mcp
{

extern bool end_of_line;  // panic/abort exit signal

extern DFState *state;  // For DFState, there can be only one
extern ALLEGRO_MUTEX *stateMutex;  // here only for when a new state is started

// Internal event queue  (allegro events used for external input)
extern InternalEvents eventq;

// For now, assuming there is only one window.
// This will be moved for implementation of multiple windows/views
extern Window *window;

// Just for main()
bool Initialize();
void MainLoop();

}  // namespace mcp

