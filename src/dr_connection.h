#pragma once

#include <plugins/dr_data.h>  // dfhack/plugin
#include <ActiveSocket.h>

class DeltaReader
    {
  public:
    DeltaReader();
    ~DeltaReader();

    bool Connect();
    bool Connected();
    void Close();

//    void * Loop(ALLEGRO_THREAD *thread);
    void * ThreadFun();

  private:
    bool go_on_then;
    CActiveSocket sock;
#if 0
    ALLEGRO_THREAD *thread;
#endif
    };

// vim: ts=4 sw=4 ai expandtab cinoptions={1s,g.5s,h.5s,N-s
