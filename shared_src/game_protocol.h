#ifndef GAME__PROTOCOL__H
#define GAME__PROTOCOL__H

typedef enum action {
  EXAMPLE = 0,
  GAME_STATUS = 1,
  UNIT_TRAINING = 2
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

struct example {
  char name[20];
  char surname[20];
};

typedef struct game_msg {
  action_t action_type;
  union {
    struct game_status status;
    struct train_units training;
    struct example test;
  } data;
} game_msg_t;

typedef struct msgbuf {
  long mtype;
  game_msg_t mdata;
} server_message_t;

#endif
