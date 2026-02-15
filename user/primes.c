#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void sieve(int left_pipe[2]) {
  close(left_pipe[1]); // 关闭写端，只读

  int prime = -1;
  int n = read(left_pipe[0], &prime, sizeof(prime)); // 读第一个数，一定是素数
  // note
  if (n < 1) {
    return;
  }

  printf("prime %d\n", prime);

  // 创建右管道和下一个进程
  int right_pipe[2];
  pipe(right_pipe);

  if (fork() == 0) {
    // 子进程：下一个筛子（进程3）
    close(right_pipe[1]); // 子进程只读
    sieve(right_pipe);    // 递归调用
  } else {
    // 父进程（进程2）：过滤并转发数据
    close(right_pipe[0]); // 父进程只写

    int num;
    while (read(left_pipe[0], &num, sizeof(num)) > 0) {
      // if (num < 2 || num > 35) {
      //   break;
      // }
      if (num % prime != 0) { // 不能被当前素数整除
        write(right_pipe[1], &num, sizeof(num));
      }
    }

    close(right_pipe[1]);
    wait(0); // 等待子进程
  }
}

int main(int argc, char *argv[]) {
  int p[2];
  pipe(p);

  if (fork() == 0) {
    // 子进程：第一个筛子
    close(p[1]);
    sieve(p);
  } else {
    // 父进程：生成数字 2-35
    close(p[0]);

    for (int i = 2; i <= 35; i++) {
      write(p[1], &i, sizeof(i));
    }

    close(p[1]);
    wait(0);
  }

  exit(0);
}
