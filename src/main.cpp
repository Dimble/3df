#include <stdio.h>
#include <stdlib.h>

#include <allegro5/allegro.h>

#include "dr_connection.h"
 
int main(int argc, char **argv){
    ALLEGRO_DISPLAY *display = 0;
    DeltaReader dr;
 
   if(!al_init()) {
      fprintf(stderr, "failed to initialize allegro!\n");
      return -1;
   }
 
   display = al_create_display(640, 480);
   if(!display) {
      fprintf(stderr, "failed to create display!\n");
      return -1;
   }
 
    al_clear_to_color(al_map_rgb(0,0,0));
 
    al_flip_display();

    dr.Connect();
 
    al_rest(100.0);
 
    al_destroy_display(display);
 
    return 0;
    }

// vim: ts=4 sw=4 ai expandtab cinoptions={1s,g.5s,h.5s,N-s
