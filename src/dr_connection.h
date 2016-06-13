#pragma once

class DFState;
class CActiveSocket;
class DeltaReader
    {
  public:
    DeltaReader(DFState *state);
    ~DeltaReader();

    bool Connect();
    bool Connected();
    void Close();
    void Shutdown();
    bool Process();

  private:
    CActiveSocket *sock;
    DFState *dfstate;
    };

// vim: ts=4 sw=4 ai expandtab cinoptions={1s,g.5s,h.5s,N-s
