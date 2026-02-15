#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
isdigit(const char *s)
{
  while (*s != '\0') {
    if ('0' <= *s && *s <= '9')
      s++;
    else
      return 0;
  }
  return 1;
}

int
main(int argc, char *argv[])
{
  int i;

  if(argc < 2){
    fprintf(2, "Usage: sleep [numbers...]\n");
    exit(1);
  }

  int sum = 0;
  for (i = 1; i < argc; i++) {
    if(isdigit(argv[i]) == 0){
      fprintf(2, "sleep: invalid time interval '%s'\n", argv[i]);
      break;
    } else {
      int x = atoi(argv[i]);
      sum += x;
    }
  }
  // printf("sleep: %d\n", sum);

  sleep(sum * 10);

  exit(0);
}
