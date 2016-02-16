#ifndef GAME__PROTOCOL__H
#define GAME__PROTOCOL__H

#define GAME_START_ID 1
#define GAME_STATUS_ID 2

/**
 * ARMY TYPES:
 * light   = 0
 * heavy   = 1
 * cavalry = 2
 * workers = 3
 */
typedef struct army {
  unsigned int light;
  unsigned int heavy;
  unsigned int cavalry;
  unsigned int workers;
} army_t;

struct game_status {
  unsigned int resources;
  army_t army;
};

struct train_units {
  short type;
  unsigned count;
};

typedef struct msgbuf {
  long mtype;
  union {
    struct game_status status;
    struct train_units train;
  } data;
} server_message_t;

#endif
