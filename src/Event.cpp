// Internal event scheme.

#include "MCP.h"
//#include "Event.h"  // global header in MCP.h

// TODO  pool of Event objects.
//       - possibly just for the very common mouse input and map updates

Event * EventQueue::GetNext ()
    {
    if ( q.size() > 0 )
        {
        Event *result = q.front();
        // ideally use a thread-safe fifo instead of having to lock here
        mcp::eventq.Lock();
        q.pop();
        mcp::eventq.Unlock();
        return result;
        }
    else 
        return nullptr;
    }

//---------------------------------------------------------------------

InternalEvents::InternalEvents ()
    {
    mutex = al_create_mutex();
    assert(mutex);
    }

InternalEvents::~InternalEvents ()
    {
    al_destroy_mutex(mutex);
    }

EventQueue * InternalEvents::AddQueue ()
    {
    EventQueue *new_queue = new EventQueue();
    Lock();
    q_list.push_back(new_queue);
    Unlock();
    return new_queue;
    }

void InternalEvents::RemoveQueue (EventQueue *eq)
    {
    assert(eq);
    Lock();
    for ( auto it = begin(q_list); it != end(q_list); it++ )
        if ( (*it) == eq )
            {
            q_list.erase(it);
            Unlock();
            return;
            }
    Unlock();
    assert(false);
    }

// Locking is optional in case the caller wishes to handle it.
// This is primarily for the case where dfstate issues a large number of updates.
void InternalEvents::Add (Event *event, bool lock)
    {
    assert(q_list.size() > 0);
    if ( lock ) Lock();

//EventQueue *eq = q_list.at(0);
//eq->q->push(event);
    q_list.at(0)->q.push(event);  // put the passed event in the first queue
    for ( auto it = begin(q_list) + 1; it != end(q_list); it++ )
        (*it)->q.push(event->clone());

    if ( lock ) Unlock();
    }

