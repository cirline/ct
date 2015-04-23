#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

/**
 * 1. create socket -- socket()
 * 2. connect server -- connect()
 * 3. read & write -- read(), write()
 */

#define NET_IPADDR  "192.168.0.187"
#define NET_PORT    8099

int main(void)
{
    int socket_fd;
    struct sockaddr_in saddr;
    int ret;
    char buf[1024];

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_fd < 0) {
        printf("create socket failure socket_fd = %d\n", socket_fd);
        return -1;
    }

    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(NET_PORT);
    saddr.sin_addr.s_addr = inet_addr(NET_IPADDR);

    ret = connect(socket_fd, (struct sockaddr *)&saddr, sizeof(saddr));
    if(ret < 0) {
        printf("connect failure, ret = %d\n", ret);
        return -1;
    }
    ret = read(socket_fd, buf, 1024);
    if(ret < 0) {
        printf("read failure. ret = %d\n", ret);
    } else {
        printf("server: %s\n", buf);
    }
    while(1) {
        scanf("%s", buf);
        ret = write(socket_fd, buf, 1024);
        if(ret < 0) {
            printf("write failure. ret = %d\n", ret);
            continue;
        }
        ret = read(socket_fd, buf, 1024);
        if(ret < 0) {
            printf("read server failure. ret = %d\n", ret);
            continue;
        }
        printf("server: %s\n", buf);
    }
    return 0;
}

