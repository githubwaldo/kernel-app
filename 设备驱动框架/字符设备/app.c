#include <fcntl.h>//open
#include <unistd.h>//read write close
#include <string.h>//strlen
#include <stdio.h>//strlen

char *ptr = "hello, world!";
char *dev = "/dev/memdev0";

int main(int argc, char *argv[])
{
    /* 文件打开 */
    int fd;
    if((fd = open(dev, O_RDWR)) == -1) {
        perror("open");
        return 0;
    }
    /* 写文件 */
    ssize_t bytes;
    if((bytes = write(fd, ptr, strlen(ptr))) == -1) {
        perror("write");
        return 0;
    }
    
    lseek(fd, (off_t)0, SEEK_SET);
    
    /* 读文件 */
    //ssize_t bytes;
    char buffer[4096]={0};
    if((bytes = read(fd, buffer, sizeof(buffer))) == -1) {
        perror("read");
        return 0;
    }
    printf("read from %s: %s\n", dev, buffer);
    close(fd);

    return 0;
}