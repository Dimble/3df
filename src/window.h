class Window
    {
  public:
    Window ()
      : display(nullptr)
        {}

    bool Init(int x, int y, int width, int height);
    void Close();

  protected:
    ALLEGRO_DISPLAY *display;
    };
