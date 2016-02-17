#ifndef IO__TOOLS__H
#define IO__TOOLS__H

char *get_command();
char remove_last_char();
int get_client_id(char **argv);
void read_command(pid_t child_pid);

#endif
