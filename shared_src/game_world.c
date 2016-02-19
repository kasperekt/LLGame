#include <string.h>
#include "game_world.h"

const int LIGHT_COST = 100;
const int HEAVY_COST = 250;
const int CAVALRY_COST = 550;
const int WORKERS_COST = 150;

army_type_t get_army_type(const char *type_str) {
  if (strcmp(type_str, "light") == 0) {
    return LIGHT;
  } else if (strcmp(type_str, "heavy") == 0) {
    return HEAVY;
  } else if (strcmp(type_str, "cavalry") == 0) {
    return CAVALRY;
  } else if (strcmp(type_str, "worker") == 0) {
    return WORKERS;
  }

  return NONE;
}

int unit_cost(army_type_t type) {
  switch (type) {
    case LIGHT: return LIGHT_COST;
    case HEAVY: return HEAVY_COST;
    case CAVALRY: return CAVALRY_COST;
    case WORKERS: return WORKERS_COST;
    default: return 0;
  }
}

char* unit_string(army_type_t type) {
  switch (type) {
    case LIGHT: return "light";
    case HEAVY: return "heavy";
    case CAVALRY: return "cavalry";
    case WORKERS: return "workers";
    default: return "";
  }
}
