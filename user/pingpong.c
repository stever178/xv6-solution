#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  // if(argc < 2){
  //   fprintf(2, "Usage: pingpong ...\n");
  //   exit(1);
  // }

  int pipe_pos[2];
  int pipe_neg[2];
  pipe(pipe_pos);
  pipe(pipe_neg);

  char onebyte_write = '1';
  // onebyte_write = argv[1][0];
  char onebyte_read = 0;
  char onebyte_receive = 0;

  // 父进程应向子进程发送一个字节；
  // 子进程应打印“<pid>: received ping”，其中<pid>为其进程ID，将管道上的字节写回父进程，然后退出；
  // 父进程应从子进程读取该字节，打印“<pid>: received pong”，然后退出。
  if (fork() == 0) {
    // read
    close(pipe_pos[1]);
    read(pipe_pos[0], &onebyte_read, 1);
    if (onebyte_read == onebyte_write) {
      printf("successful receive in child\n");
    } else {
      printf("failed to receive in child\n");
    }

    int pid = getpid();
    fprintf(1, "%d: received ping\n", pid);

    // write
    close(pipe_neg[0]);
    write(pipe_neg[1], &onebyte_read, 1);

  } else {
    // write
    close(pipe_pos[0]);
    write(pipe_pos[1], &onebyte_write, 1);

    wait(0);

    // read
    close(pipe_neg[1]);
    read(pipe_neg[0], &onebyte_receive, 1);
    if (onebyte_receive == onebyte_write) {
      printf("successful receive in parent\n");
    } else {
      printf("failed to receive in parent\n");
    }

    int pid = getpid();
    fprintf(1, "%d: received pong\n", pid);
  }

  exit(0);
}
