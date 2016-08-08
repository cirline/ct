#include <stdio.h>
#include <string.h>

#include "aes.h"

int main(void)
{
	int i;
	char buf[1024];
	char buf2[1024];
	char cbc_buf_buf[1024];
	char cbc_buf_out[1024];
	char *encrypt_str = "abcdefghijklmnopqrstuvwxyz";
	unsigned char iv[AES_BLOCK_SIZE];
	int len;

	printf("hello world.\n");

	AES_KEY key;

	/* encrypt text - block */
	/* create encrypt key */
	//AES_set_encrypt_key("hello", 256, &key);
	//printf("ro is %d\n", key.rounds);
#if 0
	for(i = 0; i < 4 * (AES_MAXNR + 1); i++){
		printf("rk[%d] is %d\n", i, key.rd_key[i]);
	}
#endif
	//AES_encrypt(encrypt_str, buf, &key);
	//printf("out buf is %s\n", buf);

	/* encrypt text - cbc  */
	/* init iv */
	AES_set_encrypt_key("hello", 256, &key);
	for(i=0; i<AES_BLOCK_SIZE; i++)
		iv[i] = 0;
	/* count encrypt length */
	if(strlen(encrypt_str) % AES_BLOCK_SIZE) {
		len = (strlen(encrypt_str) / AES_BLOCK_SIZE + 1) * AES_BLOCK_SIZE;
	} else {
		len = strlen(encrypt_str);
	}
	AES_cbc_encrypt(encrypt_str, cbc_buf_buf, len, &key, iv, AES_ENCRYPT);
	printf("cbc buf is %s\n", cbc_buf_buf);

	AES_set_decrypt_key("hello", 256, &key);
	for(i=0; i<AES_BLOCK_SIZE; i++)
		iv[i] = 0;

	AES_cbc_encrypt(cbc_buf_buf, cbc_buf_out, len, &key, iv, AES_DECRYPT);
	printf("cbc out buf2 is %s\n", cbc_buf_out);

	//AES_decrypt(buf, buf2, &key);
	//printf("out buf2 is %s\n", buf2);


	return 0;
}


