#ifndef __UTILS_UNET_H__
#define __UTILS_UNET_H__

extern int new_server_socket(int port, int backlog);
extern int new_client_socket(char *ip, int port);
extern int http_get(const char *url, char *buffer, int buffer_len);

#endif
