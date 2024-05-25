#include <stdio.h>

#include "cgen.h"

struct mygen_state {
  int s;
  unsigned long long int i, j;
};

void mygen_init (struct mygen_state *s) {
  s->s = CGEN_START;
}

unsigned long long int mygen (struct mygen_state *s) {
  unsigned long long int temp;

  cgen_begin(s->s);

  s->i = 0;
  s->j = 1;

  while (1) {
    cgen_yield(s->s, s->i + s->j);
    temp = s->i;
    s->i = s->j;
    s->j = s->j + temp; /* fibonacci sequence */
  }

  cgen_end(s->s);
  
  return -1; /* unreachable */
}

int main () {
  struct mygen_state s;
  int i;

  mygen_init(&s);
  
  for (i = 1; i <= 50; i++) {
    printf("%2i: %11llu\n", i, mygen(&s));
  }

  return 0;
}
