
#include <stdlib.h>

#include "MCP.h"
#include "DFState.h"
#include "dr_connection.h"

// FIXME  remove this and update clsocket
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <ActiveSocket.h>

/*
 *  Not much checking is done on the socket overall, as we are connecting to a
 *  local process.
 *  If that changes this will likely need some more network handling.
*/

namespace
{

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
        }
fprintf(stderr, "\n");

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

DeltaReader::DeltaReader (DFState *state)
  : sock(nullptr), dfstate(state)
    {}

DeltaReader::~DeltaReader ()
    {
    if ( sock )
        {
        Close();
        delete sock;
        }
    }

bool DeltaReader::Connect ()
    {
    if ( sock ) delete sock;
    sock = new CActiveSocket();

    if ( sock->Initialize() )
        {
        const char *host = getenv("DFHACK_DR_HOST");
        if ( !host ) host = "localhost";
        if ( sock->Open((uint8 *)host, dr_port()) )
            {
            if ( !sock->SetNonblocking() )
                {
                fprintf(stderr, "DeltaReader.SetNonblocking: %s\n", sock->DescribeError());
                // FIXME  ideally the connection should block indefinitely
                sock->SetReceiveTimeout(2, 0);
                }
            else
                {
                // Backup plan in case non blocking fails.
                // 100ms should be frequent enough unless something is added
                // that needs really fast communication
                sock->SetReceiveTimeout(0, 100 * 1000);
                }
            sock->SetOptionReuseAddr();
            return true;
            }
        else
            fprintf(stderr, "Connection to dfhack via host:%s port:%d failed: %s\n",
                    host, dr_port(), sock->DescribeError());
        }
    else
        fprintf(stderr, "socket Initialize: %s\n", sock->DescribeError());

    delete sock; sock = nullptr;
    return false;
    }

bool DeltaReader::Connected ()
    {
    return sock->IsSocketValid();
    }

void DeltaReader::Close ()
    {
    if ( sock->IsSocketValid() )
        sock->Close();
    }

void DeltaReader::Shutdown ()
    {
    if ( sock->IsSocketValid() )
        sock->Shutdown(CSimpleSocket::Both);
    }

// returns false on lost connection
bool DeltaReader::Process ()
    {
fprintf(stderr, "DeltaReader::Process\n");
    if ( sock->Select(true, false) )  // should block regardless of Nonblocking status
        {
fprintf(stderr, "DeltaReader::Selected: ");
        int32 received = sock->ReceiveAvailable(8096);
fprintf(stderr, "%d\n", received);
        if ( received > 0 )
            ProcessIncoming(sock->GetData(), received);
        else
            {
            if ( received == -1 )
                {
                fprintf(stderr, "Lost connection to dfhack: %s\n",
                        sock->DescribeError());
                return false;
                }
            }
        }
    else
        {
        if ( !sock->IsSocketValid() )
            {
            fprintf(stderr, "Lost dfhack connection: %s\n",
                    sock->DescribeError());
            return false;
            }
        }

    return true;
    }

// vim: ts=4 sw=4 ai expandtab cinoptions={1s,g.5s,h.5s,N-s
