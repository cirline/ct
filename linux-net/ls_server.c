#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define NET_PORT    8999

/**
 * 1. create socket
 * 2. bind address
 * 3. listen client
 * 5. accept connection
 * 6. read & write
 */

int main(void)
{
    int sockfd;
    int clifd;
    struct sockaddr_in saddr;
    int ret;
    char buf[1024], buf2[1024];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0) {
        printf("create sockfd failure: %d\n", sockfd);
        return -1;
    }
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(NET_PORT);
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);

    ret = bind(sockfd, (struct sockaddr *)(&saddr), sizeof(saddr));
    if(ret < 0) {
        printf("bind failure! ret = %d\n", ret);
        return -1;
    }
    ret = listen(sockfd, 5);
    if(ret < 0) {
        printf("listen failure! ret = %d\n", ret);
        return -1;
    }
    clifd = accept(sockfd, (struct sockaddr *)(&saddr), sizeof(saddr));
    if(ret < 0) {
        printf("accept failure! clifd = %d\n", clifd);
        return -1;
    }
    strcpy(buf, "hello , this is server.");
    ret = write(clifd, buf, 1024);
    if(ret < 0) {
        printf("write banner failure! ret = %d\n", ret);
        return -1;
    }
    while(1) {
        ret = read(clifd, buf, 1024);
        if(ret < 0) {
            printf("read failure! ret = %d\n", ret);
            continue;
        }
        strcpy(buf2, "You said:");
        strcat(buf2, buf);
        ret = write(clifd, buf2, 1024);
        if(ret < 0) {
            printf("write failure! ret = %d\n", ret);
            continue;
        }
    }

    return 0;
}
