
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>

#include <sys/ipc.h>
#include <sys/shm.h>

#include "deamon.h"

static int shmid;

void * init_shm(void)
{
    int id;
    void *p;

    id = shmget((key_t)SHM_KEY, sizeof(struct deamon_shm), 0666 | IPC_CREAT);
    if(id == -1) {
        perror("shmget failed. ");
        goto err_out;
    }
    p = shmat(id, 0, 0);
    if(p == (void *)-1) {
        perror("shmat failed. ");
        goto err_shmat;
    }
    shmid = id;
    return p;

err_shmat:
    shmctl(id, IPC_RMID, 0);
err_out:
    return NULL;
}

int main(int argc, char *argv[])
{
    int i, rc;
    char mbuff[64];
    struct deamon_shm *shm;

    if(argc < 2) {
        perror("request argument.\n");
        return -1;
    }

    shm = init_shm();
    if(!shm) {
        perror("init shm failed: ");
        return -1;
    }

    while(1) {
        if(shm->state == 0) {
            shm->state = 2;
            strcpy(shm->data, argv[1]);
            shm->state = 1;
            break;
        }
        sleep(5);
    }

    shmdt(shm);

    printf("end\n");

    return 0;
}

