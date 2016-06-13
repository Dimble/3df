//  3df

//  MCP, the merry control process
//  - primary application loop for input, window events, etc.
//  - starting point for major pieces of the program
//  - may de-res programs fighting for the users

#include <stdlib.h>

#include "MCP.h"
#include "DFState.h"
#include "Input.h"
#include "Window.h"

namespace mcp
{

// initialize globals
bool end_of_line = false;  // should not be used normally
InternalEvents eventq;
DFState *state = nullptr;
ALLEGRO_MUTEX *stateMutex = nullptr;
Window *window = nullptr;

// file locals
EventQueue *eventQueue = nullptr;
ALLEGRO_THREAD *stateThread = nullptr;
bool no_state = false;  // state stays in place after a disconnect or failure,
                        // set to false when a new one should be created

} // namespace mcp

namespace
{

// DFState thread

// FIXME  this ugliness exists only in your mind
//        ... least bad workaround for running a class method?
void * stateThread_kludge (ALLEGRO_THREAD *thread, void *nada)
    {
    // TODO  pass host/port from config/command args
    if ( !mcp::state->Connect() )
        return nullptr;

    // FIXME RAII this event queue.  Probably.
    {
    EventQueue *q = mcp::eventq.AddQueue();
    while ( mcp::state->Process() )
        {
        if ( !mcp::state->ProcessEvents(q) )
            break;
        }
    mcp::eventq.RemoveQueue(q);
    }

    mcp::no_state = true;
    return nullptr;
    }

void new_dfstate ()
    {
    al_lock_mutex(mcp::stateMutex);
    if ( mcp::state ) delete mcp::state;
    mcp::state = new DFState();
    al_unlock_mutex(mcp::stateMutex);

    mcp::stateThread = al_create_thread(&stateThread_kludge, nullptr);
    al_start_thread(mcp::stateThread);
    }

}  // unnamed namespace

namespace mcp
{

bool Initialize ()
    {
    if ( !al_install_keyboard() )
        {
        fprintf(stderr, "3df: error initializing keyboard\n");
        return false;
        }
    if ( !al_install_mouse() )  // FIXME  ideally, should be optional
        {
        fprintf(stderr, "3df: error initializing mouse pointer\n");
        return false;
        }

    // First, add our main internal event queue
    eventQueue = eventq.AddQueue();

    // Next, start up the render state
    // - done first to ensure the renderer gets its event queue and
    //   receives window opening events

    // Next, open UI window(s)
    window = new Window();
    if ( !window->Init(-1, -1, 800, 600) )  // TODO  starting window(s) config
        {
        fprintf(stderr, "3df: couldn't open initial window. :(\n");
        return false;
        }

    // Finally start up the DF State thread.
    // Upon connection it will fire the events that really get things going.
    stateMutex = al_create_mutex();
    new_dfstate();

    return true;
    }

void MainLoop ()
    {
    Input input;
    Event *event;

    while ( !end_of_line )
        {
        // get any pending keyboard/mouse/etc input
        input.Process();

        // get any pending window events
        // window.Process();
        // if no windows are open, fire an EXIT

        // handle resulting internal events
        while ( (event = eventQueue->GetNext()) != nullptr )
            {
            if ( event->id == event::EXIT )
                {
                if ( state )
                    state->Shutdown();
                if ( stateThread )
                    al_join_thread(stateThread, nullptr);
                // join on the render thread
                end_of_line = true;
                break;
                }

            delete event;
            }

#if NOPE
        // remove the state object if it ended
        if ( !state->Connected() )
            {
fprintf(stderr, "MCP: !state->Connected()\n");
            delete state;
            state = nullptr;
            eventq.Add(event::DISCONNECTED);
            }
#endif

        // remove any closed windows
        }
    }

}  // namespace mcp
