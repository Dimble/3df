#include "MCP.h"
#include "DFState.h"
#include "dr_connection.h"

// FIXME  rework dfstate/deltareader to do everything but the socket connect()
//        via constructor and to unwind after disconnect/failure.

DFState::DFState ()
    {
    dr = new DeltaReader(this);
    }

DFState::~DFState ()
    {
    delete dr;
    }

bool DFState::Connect ()
    { return dr->Connect(); }

bool DFState::Connected ()
    { return dr->Connected(); }

void DFState::Shutdown ()
    { dr->Shutdown(); }

bool DFState::Process ()
    {
    if ( !dr->Process() )
        return false;

    // ...

    return true;
    }

bool DFState::ProcessEvents (EventQueue *q)
    {
    Event *event;

    while ( (event = q->GetNext()) != nullptr )
        {
        if ( event->id == event::EXIT )
            {
fprintf(stderr, "dfstate: Time to exit!  Closing connection to DFHack.\n");
            dr->Close();
            return false;
            }

        delete event;
        }

    return true;
    }

