#include <fcntl.h>
#include <linux/fs.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
  int fd;
  unsigned long long numblocks=0;

  fd = open(argv[1], O_RDONLY);
  ioctl(fd, BLKGETSIZE64, &numblocks);
  close(fd);
  printf("Number of bytes: %llu, this makes %.3f GB\n",
     numblocks,
     (double)numblocks / (1024 * 1024 * 1024));
}

