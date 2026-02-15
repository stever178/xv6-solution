#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int p[2];
  pipe(p);

  int a = 10;
  write(p[1], &a, sizeof(a));

  int b = 0;
  read(p[0], &b, sizeof(a));
  printf("b: %d\n", b);

  exit(0);
}
