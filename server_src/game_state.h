#ifndef GAME__STATE__H
#define GAME__STATE__H

typedef struct army {
  unsigned int light;
  unsigned int heavy;
  unsigned int cavalry;
  unsigned int workers;
} army_t;

typedef struct game_state {
  unsigned int resources;
  army_t army;
} game_state_t;

#endif
