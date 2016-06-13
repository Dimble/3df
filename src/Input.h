class Input
    {
  public:
    Input();
    ~Input();

    void Process();

  private:
    //void AddMouseEvent(ALLEGRO_EVENT *event);

    ALLEGRO_EVENT_QUEUE *al_events;
    };

