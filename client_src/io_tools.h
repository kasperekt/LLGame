#ifndef IO__TOOLS__H
#define IO__TOOLS__H

typedef enum command {
  WRONG_COMMAND = 0,
  TRAIN_COMMAND = 1,
  ATTACK_COMMAND = 2
} command_t;

void clear_terminal();
command_t get_command();
char *remove_last_char(char *str);
int get_client_id(char **argv);
void read_command(pid_t child_pid);

#endif
