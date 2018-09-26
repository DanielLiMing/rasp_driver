#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    /* code */
    int fd;
    if(argc < 3)
    {
        printf("Para error\r\n");
        exit(1);
    }
    printf("driver para: %s  %s\r\n",argv[1],argv[2]);

    char *hello = "/dev/hello";
    fd = open(hello, O_RDWR | O_NOCTTY | O_NDELAY); 
    if(fd < 0)
    {
        printf("open %s failed\r\n",hello);
        exit(1);
    }
    else
    {
        printf("%s fd is %d \r\n",hello,fd);
        ioctl(fd,atoi(argv[1]),atoi(argv[2]));
    }
    close(fd);
    return 0;
}
