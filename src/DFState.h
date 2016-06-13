//  Dwarf Fortress State.

//  This is a public data store that maintains the state of a dwarf fortress game.

#include <plugins/dr_data.h>

class DeltaReader;
class DFState
    {
  public:
    DFState();
    ~DFState();

    dr::Status status;
    dr::world::Data world;
    dr::map::Data map;

#ifdef DELETETHIS
    void attachReader (DeltaReader *delta_reader)
        { dr = delta_reader; }
    void detachReader ()
        { dr = nullptr; }
#endif

    bool Connected();

    // TODO  privatize and friend the appropriate functions in MCP.cpp
    bool Connect();
    bool Process();
    bool ProcessEvents(EventQueue *q);
    void Disconnect () { exit_thread = true; }  // doesn't immediately happen
    void Shutdown();

  private:
    DeltaReader *dr;
    bool exit_thread;
    };

