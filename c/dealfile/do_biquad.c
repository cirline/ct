#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#define ARRAY_SIZE(arr)     (sizeof(arr)/sizeof(arr[0]))

struct biquad_data {
    int addr[7];
    int val[7];
};

struct hz_data {
    int count;
    struct biquad_data data[21];
};

enum {
    F80     = 0,
    F100,
    F200,
    F300,
    F500,
    F1000,
    F2000,
    F3000,
    F5000,
    F10000,
    F15000,

    FEND,
};

enum string_type {
    TYPE_HZ = 0,
    TYPE_DB,
    TYPE_DATA,

    TYPE_NONE,
};

const char *freq_desc[] = {
    "80hz",
    "100hz",
    "200hz",
    "300hz",
    "500hz",
    "1000hz",
    "2000hz",
    "3000hz",
    "5000hz",
    "10000hz",
    "15000hz",
};

enum {
    DATA_1 = 1,
    DATA_6 = 6,
};

int hz_data_count[] = {
    6,
    6,
    6,
    6,
    6,
    1,
    6,
    1,
    6,
    1,
    6,
};

static struct hz_data eq_data[FEND];

enum string_type string_check(char *s)
{
    int len;

    if(strncmp(s, "address", 7) == 0)
        return TYPE_DATA;

    len = strlen(s);
    if(strncmp(s + len - 2, "hz", 2) == 0)
        return TYPE_HZ;
    else if(strncmp(s + len - 2, "db", 2) == 0)
        return TYPE_DB;
    else
        return TYPE_NONE;
}

int freq_to_index(char *freq)
{
    int i;
    for(i = 0; i < ARRAY_SIZE(freq_desc); i++) {
        if(strcmp(freq, freq_desc[i]) == 0)
            return i;
    }

    return -1;
}

int db_to_index(char *s_db)
{
    int db;
    sscanf(s_db, "%ddb", &db);

    return db + 10;
}

unsigned long fo_size(int fd)
{
    return lseek(fd, 0, SEEK_END);
}

int readline(int fd, char *buffer, int *position)
{
    char tmp[1024];
    int i, ret;
    int split;

    if(*position >= fo_size(fd))
        return -1;
    lseek(fd, *position, SEEK_SET);

    ret = read(fd, tmp, 1024);
    for(i = 0; i < ret; i++) {
        buffer[i] = tmp[i];
        if(buffer[i] == '\n') {
            buffer[i] = 0;
            break;
        }
    }
    split = buffer[i-1] == '\r' ? 2 : 1;
    if(i == ret)
        split = 0;
    if(i == ret || buffer[i-1] == '\r') {
        i--;
        buffer[i] = 0;
    }
    *position += i + split;

    return i;
}

int load_eqdata(void)
{
    int i;
    int fd;
    int position;
    int ret;
    char buffer[2048];
    int current_hz, current_db;
    int di;
    int address, value;

    fd = open("base.txt", O_RDONLY);
    if(fd < 0) {
        printf("open failed: %s\n", strerror(errno));
        return -1;
    }

    position = 0;
    current_hz = 0;
    current_db = 0;
    do {
        ret = readline(fd, buffer, &position);
        printf("buffer: %s\n", buffer);
        switch(string_check(buffer)) {
            case TYPE_HZ:
                current_hz = freq_to_index(buffer);
                eq_data[current_hz].count = hz_data_count[current_hz];
                printf("set current_hz = %d\nn", current_hz);
                break;
            case TYPE_DB:
                current_db = db_to_index(buffer);
                printf("set current_db = %d\n", current_db);
                di = 0;
                break;
            case TYPE_DATA:
                sscanf(buffer, "address 0x%x data 0x%x", &address, &value);
                printf("%d, %d, %d, 0x%x, 0x%x\n", current_hz, current_db, di, address, value);
                eq_data[current_hz].data[current_db].addr[di] = address;
                eq_data[current_hz].data[current_db].val[di] = value;
                printf("address 0x%x = 0x%x\n", eq_data[current_hz].data[current_db].addr[di], eq_data[current_hz].data[current_db].val[di]);
                di++;
                break;
        }
    } while(ret > 0);
    return 0;
}

void eqshow(int f, int d)
{
    int i;

    for(i = 0; i < eq_data[f].count; i++) {
        printf("address 0x%x = 0x%x\n", eq_data[f].data[d].addr[i], eq_data[f].data[d].val[i]);
    }
}

int main(int argc, char * argv[])
{

    int f, d;

    load_eqdata();

    if(argc == 3) {
        f = atoi(argv[1]);
        d = atoi(argv[2]);

        printf("eqshow \n\n");
        eqshow(f, d);
    }

    return 0;
}

