#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

static unsigned long map[][3] = {
    {1, 461920,     262136},
    {2, 724064,     16},
    {3, 724712,     16},
    {4, 728664,     257536},
    {5, 986208,     16},
    {6, 1248352,    16},
    {7, 1249000,    16},
    {8, 1252952,    257536},
    {9, 1510496,    16},
    {10,1772640,    16},
    {11,1773288,    16},
    {12,1777240,    138600},
    {13,2034784,    16},
    {14,2296928,    16},
    {15,2297576,    16},
    {16,2559072,    16},
    {17,2821216,    16},
    {18,2821864,    16},
    {19,2825816,    152680},
};

int main(void)
{
    int fd_in, fd_out;
    char buffer[512];
    char zero_buffer[512];
    unsigned long count, empty_count, filled_size, empty_size;
    unsigned long i, j;
    int ret;
    char filename[32];

    fd_out = open("system_all.img", O_CREAT | O_RDWR);
    if(fd_out < 0) {
        perror("open fd_out.");
        return -1;
    }

    memset(buffer, 0, 512);
    memset(zero_buffer, 0, 512);

    filled_size = 0;
    empty_size = 0;
    for(i = 0; i < 19; i++) {
        sprintf(filename, "system_%ld.img", map[i][0]);
        printf("dealing file: %s ...\n", filename);

        count = 0;

        fd_in = open(filename, O_RDONLY);
        if(fd_in < 0) {
            perror("open file :");
            return -1;
        }

        for(j = 0; j < map[i][2]; j++) {
            ret = read(fd_in, buffer, 512);
            if(ret < 0) {
                perror("read :");
                return -1;
            }
            ret = write(fd_out, buffer, 512);
            if(ret < 0) {
                perror("write error: ");
                return -2;
            }
            count += ret;
        }
        printf("loop %ld write size = %ld/%ld\n\n", i, count, map[i][2] * 512);
        filled_size += count;

        if(i > 0) {
            empty_count = map[i][1] - map[i-1][1] - map[i-1][2];
            printf("loop %ld empty_count = %ld\n\n", i, empty_count);
            count = 0;
            for(j = 0; j < empty_count; j++) {
                ret = write(fd_out, zero_buffer, 512);
                if(ret < 0) {
                    perror("zero_buffer write: ");
                    return -1;
                }
                count += ret;
            }
            printf("loop %ld write zero size = %ld/%ld\n\n", i, count, empty_count * 512);
            empty_size += count;
        }

    }
    printf("all write size = %ld\n", filled_size + empty_size);
    printf("sector end - start + tail = %ld byte \n", (map[18][1] - map[0][1] + map[18][2]) * 512 );

    return 0;

}

