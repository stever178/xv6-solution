#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void
find(char *path, char *file_name)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }
  
  // printf("try find %s %s\n", path, file_name);
  switch(st.type){
  case T_FILE:
    printf("find: first arg should be a path\n");
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("find: path too long\n");
    }

    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';

    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
      if(de.inum == 0)
        continue;
      if (strcmp(de.name, ".") == 0)
        continue;
      if (strcmp(de.name, "..") == 0) 
        continue;

      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;

      int fd_child = open(buf, 0);
      if (fd_child < 0) {
        fprintf(2, "find: cannot open %s\n", buf);
        return;
      }

      if(fstat(fd_child, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", buf);
        close(fd_child);
        return;
      }

      if (st.type == T_DIR) {
        close(fd_child);
        find(buf, file_name);
      } else {
        close(fd_child);
        if (strcmp(file_name, p) == 0) {
          printf("%s\n", buf);
        }
      }
    }
    break;

  }

  close(fd);
}

int
main(int argc, char *argv[])
{
  if(argc < 3){
    fprintf(2, "usage: find [path] [file]\n");
    exit(1);
  }

  find(argv[1], argv[2]);

  exit(0);
}
