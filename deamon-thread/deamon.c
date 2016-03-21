#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>

static int fd;
static int this_running;

void kill_signal(int arg)
{
    char kbuff[64];

    sprintf(kbuff, "kill_signal argument = %d\n", arg);
    if(fd)
        write(fd, kbuff, strlen(kbuff));

    this_running = 0;
}

int main(void)
{
    int pid;
    int i, rc;
    char mbuff[64];

    pid = fork();

    if(pid < 0) {
        printf("fork failed.\n");
        return -1;
    } else if(pid > 0) {
        printf("parent thread exit.\n");
        return 0;
    }

    /* set child thread */
    setsid();

    /* ch dir */
    chdir("/tmp");

    fd = open("deamon_test.log", O_RDWR | O_CREAT | O_APPEND);
    if(fd < 0) {
        perror("open failed: ");
        return -1;
    }

    /* close fd */
    for(i = 0; i < 3; i++)
        close(i);

    /* register signal */
    signal(SIGTERM, kill_signal);

    this_running = 1;

    while(this_running++) {
        sprintf(mbuff, "this loop %d\n", this_running);
        write(fd, mbuff, strlen(mbuff));
        sleep(5);
    }

    sprintf(mbuff, "loop end\n");
    write(fd, mbuff, strlen(mbuff));

    close(fd);

    return 0;
}

