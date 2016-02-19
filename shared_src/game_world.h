#ifndef GAME__WORLD__H
#define GAME__WORLD__H

extern const int LIGHT_COST;
extern const int HEAVY_COST;
extern const int CAVALRY_COST;
extern const int WORKERS_COST;

extern const int LIGHT_TIME;
extern const int HEAVY_TIME;
extern const int CAVALRY_TIME;
extern const int WORKERS_TIME;

typedef enum army_type {
  NONE = 0,
  LIGHT = 1,
  HEAVY = 2,
  CAVALRY = 3,
  WORKERS = 4
} army_type_t;

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

army_type_t get_army_type(const char *type_str);
int unit_cost(army_type_t type);
int unit_training_time(army_type_t type);
char *unit_string(army_type_t type);

#endif
