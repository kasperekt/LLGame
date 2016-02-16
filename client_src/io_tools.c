#include <unistd.h>
#include <stdio.h>

void read_stdin() {
  char c;
  while ((c = getchar()) != '\n') {
    printf("%c", c);
  }
}
