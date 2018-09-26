#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>

int main(int argc, char **argv)
{
    int on;
    int fd;
    if (argc != 2 || sscanf(argv[1],"%d", &on) != 1 ||on < 0 || on > 1 ) 
    { 
        fprintf(stderr, "Usage:%s 0|1\n",argv[0]); 
        exit(1); 
    } 
    fd = open("/dev/my_leds", 0);
    if(fd < 0)
    {
        perror("open device leds");
        exit(1);
    }
    if(on)
    {
        printf("turn on leds\r\n");
        ioctl(fd, 1);
    }
    else
    {
        printf("turn off leds\r\n");
        ioctl(fd, 0);
    }
    close(fd);
    return 0;
}