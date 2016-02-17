#ifndef GAME__PROTOCOL__H
#define GAME__PROTOCOL__H
#include "game_world.h"

struct game_status {
  int resources;
  army_t army;
};

struct train_units {
  int type;
  int count;
};

typedef enum action {
  CONNECT = 1,
  GAME_START = 2,
  GAME_STATUS = 3,
  UNIT_TRAINING = 4
} action_t;

typedef struct game_msg {
  action_t action_type;
  union {
    int client_id;
    struct game_status status;
    struct train_units training;
  } data;
} game_message_t;

typedef struct msgbuf {
  long mtype;
  game_message_t mdata;
} server_message_t;

#endif
