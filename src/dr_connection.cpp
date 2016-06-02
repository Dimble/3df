// FIXME  remove this and update clsocket
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdlib.h>

#include <allegro5/allegro.h>
#include <ActiveSocket.h>
#include "dr_connection.h"
//#include <plugins/dr_data.h>

/*
 *  Not much checking is done on the socket here, as we are connecting to a
 *  local process.
 *  If that changes this will likely need some more network handling.
*/

namespace
{

// FIXME  this ugly kludge exists only in your mind
//        ... not sure this can be fixed without adding worse workarounds..
//void * dr_loop (ALLEGRO_THREAD *thread, void *dr_ptr)
void * dr_loop (void *dr_ptr)
    {
    DeltaReader *dr = reinterpret_cast<DeltaReader *>(dr_ptr);
    return dr->ThreadFun();
    }

void ProcessIncoming (uint8 *data, int32 count)
    {
    int index = 0;
fprintf(stderr, "ProcessIncoming (%d)\n  ", count);

    while ( index < count )
        {
        switch ( data[index] )
            {
            case dr::STATUS:
fprintf(stderr, " %d:%d ", index, data[index]);
                break;
            case dr::WORLD:
fprintf(stderr, " %d:%d ", index, data[index]);
                break;
            case dr::MAP_PARAMETERS:
fprintf(stderr, " %d:%d ", index, data[index]);
                break;
            case dr::MAP_BLOCK:
fprintf(stderr, " %d:%d ", index, data[index]);
                break;
            case dr::MATERIAL_LIST:
fprintf(stderr, " %d:%d ", index, data[index]);
                break;
            case dr::MAP_UPDATE:
fprintf(stderr, " %d:%d ", index, data[index]);
                break;
            }
        index++;
fprintf(stderr, "\n");
        }

    }

uint16 dr_port ()
    {
    const char *str;
    int num;

    str = getenv(ENV_DFHACK_DR_PORT);
    num = str ? atoi(str) : 0;
    if ( num > 0 && num < UINT16_MAX )
        return (uint16)num;

    str = getenv(ENV_DFHACK_PORT);
    num = str ? atoi(str) : 0;
    if ( num > 0 && num < (UINT16_MAX - 1) )
        return (uint16)num + 1;

    return dr::DFHACK_DR_PORT;
    }
}

// -------------------------------------------------------------------

DeltaReader::DeltaReader ()
  : go_on_then(true)
    {
    }

DeltaReader::~DeltaReader ()
    {
    if ( sock.IsSocketValid() )
        Close();
    }

bool DeltaReader::Connected ()
    {
    return sock.IsSocketValid();
    }

void DeltaReader::Close ()
    {
    sock.Close();
    go_on_then = false;
    // Send a 'quitting!' message here if there is ever a reason to do that
    }

//void * DeltaReader::Loop (ALLEGRO_THREAD *thread)
void * DeltaReader::ThreadFun ()
    {
    while ( go_on_then )
        {
        if ( sock.Select() )  // blocks
            {
            int32 received = sock.Receive(8096);
            if ( received > 0 )
                ProcessIncoming(sock.GetData(), received);
            else
                {
                if ( received == -1 )
                    fprintf(stderr, "Lost connection to dfhack: %s\n",
                            sock.DescribeError());
                break;
                }
            }
        else
            {
            if ( !sock.IsSocketValid() )
                {
                break;
                }
            }
        }
    return 0;
    }

bool DeltaReader::Connect ()
    {
    bool ok = false;
    const char *host;
    
    host = getenv("DFHACK_DR_HOST");
    if ( !host ) host = "localhost";

    if ( !sock.Initialize() )
        {
        fprintf(stderr, "socket Initialize: %s\n", sock.DescribeError());
        return false;
        }

    if ( sock.Open((uint8 *)host, dr_port()) )
        ok = true;
    else
        fprintf(stderr, "Connection to dfhack via host:%s port:%d failed: %s\n",
                host, dr_port(), sock.DescribeError());

    if ( !sock.SetNonblocking() )
        fprintf(stderr, "DeltaReader.SetNonblocking: %s\n", sock.DescribeError());
    sock.SetOptionReuseAddr();
    sock.SetReceiveTimeout(0, 25 * 1000);  // just in case, 25ms

    if ( ok )
        {
#if 0
        thread = al_create_thread(&dr_loop, this);
        al_start_thread(thread);
#endif        
        al_run_detached_thread(&dr_loop, this);
        }

    return ok;
    }

// vim: ts=4 sw=4 ai expandtab cinoptions={1s,g.5s,h.5s,N-s
