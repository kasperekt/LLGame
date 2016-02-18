#ifndef GAME__PROTOCOL__H
#define GAME__PROTOCOL__H
#include "game_world.h"

typedef struct game_status {
  int resources;
  army_t army;
} game_status_t;

typedef struct train_units {
  int type;
  int count;
} training_t;

typedef struct attack {
  short attacker_id;
  army_t army;
} attack_t;

typedef enum action {
  CONNECT = 1,
  GAME_START = 2,
  GAME_STATUS = 3,
  UNIT_TRAINING = 4,
  ATTACK = 5
} action_t;

typedef struct game_msg {
  action_t action_type;
  union {
    int client_id;
    game_status_t status;
    training_t training;
    attack_t attack;
  } data;
} game_message_t;

typedef struct msgbuf {
  long mtype;
  game_message_t mdata;
} server_message_t;

#endif
