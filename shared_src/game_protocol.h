#ifndef GAME__PROTOCOL__H
#define GAME__PROTOCOL__H

typedef enum action {
  CONNECT = 1,
  GAME_STATUS = 2,
  UNIT_TRAINING = 3
} action_t;

typedef enum recipient {
  SERVER = 1,
  PLAYER_ONE = 2,
  PLAYER_TWO = 3
} recipient_t;

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

struct game_status {
  int resources;
  army_t army;
};

struct train_units {
  int type;
  int count;
};

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
