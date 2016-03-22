#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>

#include <sys/ipc.h>
#include <sys/shm.h>

#include "deamon.h"

static int fd;
static int this_running;
static int shmid;

void logd(const char * msg)
{
    if(fd > 0)
        write(fd, msg, strlen(msg));
}

void kill_signal(int arg)
{
    char kbuff[64];

    sprintf(kbuff, "kill_signal argument = %d\n", arg);
    if(fd)
        write(fd, kbuff, strlen(kbuff));

    this_running = 0;
}

void * init_shm(void)
{
    int id;
    void *p;

    id = shmget((key_t)SHM_KEY, sizeof(struct deamon_shm), 0666 | IPC_CREAT);
    if(id == -1) {
        logd("shmget failed.\n");
        goto err_out;
    }
    p = shmat(id, 0, 0);
    if(p == (void *)-1) {
        logd("shmat failed.\n");
        goto err_shmat;
    }
    shmid = id;
    return p;

err_shmat:
    shmctl(id, IPC_RMID, 0);
err_out:
    return NULL;
}

int main(void)
{
    int pid;
    int i, rc;
    char mbuff[64];
    struct deamon_shm *shm;

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

    shm = init_shm();
    if(!shm) {
        logd("init shm failed.\n");
        return -1;
    }

    this_running = 1;
    while(this_running++) {
        if(shm->state == 1) {
            shm->state = 2;
            sprintf(mbuff, "shm: %s\n", shm->data);
            write(fd, mbuff, strlen(mbuff));
            shm->state = 0;
            if(strcmp(shm->data, "deamon_exit") == 0)
                this_running = 0;
        }
        sprintf(mbuff, "loopping : shm state = %d, data = %s\n", shm->state, shm->data);
        write(fd, mbuff, strlen(mbuff));
        sleep(5);
    }

    shmdt(shm);
    shmctl(shmid, IPC_RMID, 0);

    sprintf(mbuff, "loop end\n");
    write(fd, mbuff, strlen(mbuff));

    close(fd);

    return 0;
}

