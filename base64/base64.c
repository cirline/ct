#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const unsigned char * mapptr = "ABCDEFGHIJKLMNOPQRSTUVWXYZ\
                                abcdefghijklmnopqrstuvwxyz\
                                0123456789+/";

unsigned char * base64_encode(const unsigned char * in_code, long len)
{
    unsigned int current;
    int i;
    unsigned char *buffer;
    int buffer_size;
    int buffer_pos;

    if(len <= 0)
        return NULL;
    buffer_size = len / 3 * 4;
    if(len % 3 > 0)
        buffer_size += 4;
    buffer_size++;
    printf("buffer_size = %d\n", buffer_size);

    buffer = malloc(buffer_size);
    if(!buffer) {
        printf("encode: malloc buffer failure\n");
        return NULL;
    }

    buffer_pos = 0;
    for(i = 0; i < len - 2; i += 3) {
        current = (in_code[i] << 16) | (in_code[i+1] << 8) | (in_code[i+2] << 0);
        buffer[buffer_pos++] = mapptr[(current & (0x3f << 18)) >> 18];
        buffer[buffer_pos++] = mapptr[(current & (0x3f << 12)) >> 12];
        buffer[buffer_pos++] = mapptr[(current & (0x3f << 6)) >> 6];
        buffer[buffer_pos++] = mapptr[(current & (0x3f << 0)) >> 0];
    }
    switch(len - i) {
        case 2:
            current = (in_code[i] << 16) | (in_code[i+1] << 8);
            buffer[buffer_pos++] = mapptr[(current & (0x3f << 18)) >> 18];
            buffer[buffer_pos++] = mapptr[(current & (0x3f << 12)) >> 12];
            buffer[buffer_pos++] = mapptr[(current & (0x3f << 6)) >> 6];
            buffer[buffer_pos++] = '=';
            break;
        case 1:
            current = in_code[i] << 8;
            buffer[buffer_pos++] = mapptr[(current & (0x3f << 10)) >> 10];
            buffer[buffer_pos++] = mapptr[(current & (0x3f << 4)) >> 4];
            buffer[buffer_pos++] = '=';
            buffer[buffer_pos++] = '=';
            break;
    }
    buffer[buffer_pos] = 0;

    return buffer;
}

unsigned char * base64_decode(unsigned char * out_code)
{}


int main(void)
{
    long i;
    printf("map table length: %lu\n", strlen(mapptr));

    for(i = 0; i < 16; i++)
        printf("length=%ld, encode: %s\n", i, encode(mapptr, i));

    return 0;
}

