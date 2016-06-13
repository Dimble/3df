// Internal event scheme.

// This is the central event queue that external events are converted to and
// then dispatched, or more accurately, queried.
// The allegro event system could be used but is awkward for this purpose.

// This queue maintains a primary queue and individual queues, primarily for
// the purpose of thread access.
// - A new queue is added with AddQueue(any identifying pointer)
// - Events shall then be obtained from that queue by passing that same pointer.
// - Queue automatically duplicates new events to all queues.

#include <queue>

// TODO  possibly replace the id with something more cleverer
namespace event
{
enum Id : char
    {
    EXIT
#ifdef NOPE
    SKIP_UPDATES,
    RESUME_UPDATES,
    WINDOW_CLOSE,
    WINDOW_MINIMIZE,
    WINDOW_RESIZE,
    WINDOW_FOCUSED,
    WINDOW_UNFOCUSED,
    WINDOW_MOUSE
#endif
    };
}

class Event
    {
  public:
    event::Id id;

    Event (event::Id eventId)
      : id(eventId)
        {}

    virtual ~Event () {}

    virtual Event * clone () { return new Event(*this); }
    };

#ifdef NOPE
class WindowEvent : Event
    {
    Window window;

    int x, y;
    int width, height;
    pointer and button status stuff
    };
#endif

class EventQueue
    {
    friend class InternalEvents;

  public:
    Event * GetNext();

  private:
    std::queue<Event *> q;
    };

class InternalEvents
    {
  public:
    InternalEvents();
    ~InternalEvents();

    EventQueue * AddQueue();
    void RemoveQueue(EventQueue *eq);

    void Add (event::Id id, bool lock=true)
        { Add(new Event(id), lock); }
    void Add(Event *event, bool lock=true);
        
    ALLEGRO_MUTEX *mutex;
    void Lock () { al_lock_mutex(mutex); }
    void Unlock () { al_unlock_mutex(mutex); }

  private:
    std::vector<EventQueue *> q_list;
    };

