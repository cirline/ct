#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define NET_PORT    8099

/** server
 * 1. create server fd -- socket()
 * 2. bind fd & address -- bind()
 * 3. listen client -- listen()
 * 4. accept the require -- accept()
 * 5. read & write -- read() write()
 * ex. inet_ntoa()
 */

int main(int argc, char *argv[])
{
    int server_fd;
    int client_fd;
    struct sockaddr_in saddr, caddr;
    int ret;
    char buf[1024] = "hello world.!";
    char buf2[1024];
    int len;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd < 0) {
        printf("create socket failure. server_fd = %d\n", server_fd);
        return -1;
    }

    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(NET_PORT);
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    ret = bind(server_fd, (struct sockaddr *)(&saddr), sizeof(saddr));
    if(ret) {
        printf("bind addr & port failure. ret = %d\n", ret);
        return -1;
    }
    ret = listen(server_fd, 5);
    if(ret) {
        printf("listen error: ret = %d\n", ret);
        return -1;
    } else {
        printf("listening ...\n");
    }
    client_fd = accept(server_fd, (struct sockaddr *)(&caddr), &len);
    if(client_fd < 0) {
        printf("accept error: client_fd = %d: %s\n", client_fd, strerror(errno));
        return 0;
    }
    ret = write(client_fd, buf, 32);
    if(ret < 0){
        printf("write error!\n");
    }
    while(1){
        ret = read(client_fd, buf, 1024);
        if(ret < 0) {
            printf("read error!\n");
            continue;
        }
        strcpy(buf2, "you said:");
        strcat(buf2, buf);
        ret = write(client_fd, buf2, 1024);
        if(ret < 0) {
            printf("write client error!\n");
        }
        if(strcmp(buf, "exit") ==  0) {
            break;
        }
    };
    strcpy(buf2, "bye!");
    ret = write(client_fd, buf2, 1024);
    if(ret < 0) {
        printf("write client error!\n");
    }

    return 0;
}

