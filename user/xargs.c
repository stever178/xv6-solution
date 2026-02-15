#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int i;

  // parse arguments
  for (i = 1; i < argc;) {
    if (argv[i][0] == '-') {
      i += 2;
    } else {
      break;
    }
  }
  int pos_valid_args = i + 1;
  int argc_cli = argc - pos_valid_args;

  // read from stdin
  char buf[512];
  char c = 0;
  int argc_input = 0;
  for (i = 0; read(0, &c, 1) > 0;) {
    if (c == '\n') {
      argc_input++;
      buf[i++] = '\n';
    } else {
      buf[i++] = c;
    }

    if (i == sizeof(buf)) {
      printf("command line too long\n");
      exit(1);
    }
  }
  // printf("command line: %s\n", buf);

  //
  int total_argc = 1 + 1;
  total_argc += argc_cli;
  
  char **args = malloc(sizeof(char *) * total_argc);
  // args[0] 应该是命令名
  args[0] = malloc(strlen(argv[pos_valid_args - 1]) + 1);
  strcpy(args[0], argv[pos_valid_args - 1]);

  for (i = 0; i < argc_cli; i++) {
    args[i + 1] = malloc(strlen(argv[pos_valid_args + i]) + 1);
    strcpy(args[i + 1], argv[pos_valid_args + i]);
  }
  args[argc_cli + 1] = malloc(512);

  //
  int buf_id = 0;
  for (i = 0; i < argc_input; i++) {
    int args_id = 0;
    for (; buf_id < strlen(buf) && buf[buf_id] != '\n';) {
      args[argc_cli + 1][args_id++] = buf[buf_id++];
    }
    args[argc_cli + 1][args_id] = '\0';
    buf_id++;

    // printf("cmd: [%s]\n", argv[pos_valid_args - 1]);
    // for (int j = 0; j < total_argc; j++) {
    //   printf("args[%d]: %s\n", j, args[j]);
    // }

    if (fork() == 0) {
      exec(argv[pos_valid_args - 1], args);
      fprintf(2, "exec failed\n");
      exit(1);  // exec 失败时退出子进程
    } else {
      wait(0);
    }
  }

  // free args
  for (i = 0; i < total_argc; i++) {
    free(args[i]);
  }
  free(args);

  exit(0);
}
