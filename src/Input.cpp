// 3df
//
// Input loop

#include "MCP.h"
#include "Input.h"

Input::Input ()
    {
    al_events = al_create_event_queue();
    al_register_event_source(al_events, al_get_keyboard_event_source());
    al_register_event_source(al_events, al_get_mouse_event_source());
    }

Input::~Input ()
    {
    // docs say this is done w/destroy
    //al_unregister_event_source(al_events, al_get_keyboard_event_source());
    //al_unregister_event_source(al_events, al_get_mouse_event_source());
    al_destroy_event_queue(al_events);
    }

#ifdef TODO
void Input::AddMouseEvent (ALLEGRO_EVENT *event)
    {
    }
#endif

void Input::Process ()
    {
    ALLEGRO_EVENT event;

    while ( al_get_next_event(al_events, &event) )
        {
        if ( event.any.source == al_get_keyboard_event_source() )
            {
            if ( event.keyboard.type == ALLEGRO_EVENT_KEY_CHAR )
                {
                unsigned int anymods = event.keyboard.modifiers;
                // clear these first
                anymods &= ~(ALLEGRO_KEYMOD_SCROLLLOCK |
                             ALLEGRO_KEYMOD_NUMLOCK |
                             ALLEGRO_KEYMOD_CAPSLOCK);
                               
                switch ( event.keyboard.keycode )
                    {
                    // FIXME  use a proper keybinding system
                    case ALLEGRO_KEY_Q:
                        if ( (event.keyboard.modifiers & anymods) == 
                             ALLEGRO_KEYMOD_CTRL )
                            mcp::eventq.Add(event::EXIT);
                        break;

                    default:
                        break;
                    }
                }
            }
        else if ( event.any.source == al_get_mouse_event_source() )
            {
            }
        else
            assert(false);
            
        }
    }

