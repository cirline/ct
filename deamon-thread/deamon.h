#ifndef __DEAMON_H_CT__
#define __DEAMON_H_CT__

#define SHM_KEY     324023

struct deamon_shm {
    int state;

    char data[1024];
};


#endif
