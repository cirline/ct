#include <stdio.h>

#include "aes.h"

int main(void)
{
	int i;
	char buf[1024];
	char buf2[1024];

	printf("hello world.\n");

	AES_KEY key;

	AES_set_encrypt_key("hello", 256, &key);

	printf("ro is %d\n", key.rounds);

	for(i = 0; i < 4 * (AES_MAXNR + 1); i++){
		printf("rk[%d] is %d\n", i, key.rd_key[i]);
	}

	AES_encrypt("world", buf, &key);
	printf("out buf is %s\n", buf);

	AES_set_decrypt_key("hello", 256, &key);

	AES_decrypt(buf, buf2, &key);
	printf("out buf2 is %s\n", buf2);


	return 0;
}


