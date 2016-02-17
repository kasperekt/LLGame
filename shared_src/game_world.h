#ifndef GAME__WORLD__H
#define GAME__WORLD__H

/**
 * ARMY TYPES:
 * light   = 0
 * heavy   = 1
 * cavalry = 2
 * workers = 3
 */
typedef struct army {
  int light;
  int heavy;
  int cavalry;
  int workers;
} army_t;

#endif
