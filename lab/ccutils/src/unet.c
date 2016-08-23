#define DEBUG

#include <string.h>
#include <errno.h>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "header.h"

int new_server_socket(int port, int backlog)
{
	int fd;
	int rc;
	int on;
	struct sockaddr_in addr;

	fd = socket(AF_INET, SOCK_STREAM, 0);
	if(fd < 0) {
		pr_err("create socket failed: %s\n", strerror(errno));
		return fd;
	}

	on = 1;
	rc = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	if(rc < 0) {
		pr_err("setsockopt failed: %s\n", strerror(errno));
		goto err_setsockopt;
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	rc = bind(fd, (struct sockaddr *) &addr, sizeof(struct sockaddr_in));
	if(rc < 0) {
		pr_err("bind failed: %s\n", strerror(errno));
		goto err_bind;
	}
	rc = listen(fd, backlog);
	if(rc < 0) {
		pr_err("listen failed: %s\n", strerror(errno));
		goto err_listen;
	}
	return fd;

err_listen:
err_bind:
err_setsockopt:
	close(fd);

	return rc;
}

int new_client_socket(char *ip, int port)
{
	int fd;
	int rc;
	struct sockaddr_in addr;

	fd = socket(AF_INET, SOCK_STREAM, 0);
	if(fd < 0) {
		pr_err("socket failed: %s\n", strerror(errno));
		return fd;
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip);

	rc = connect(fd, (struct sockaddr *) &addr, sizeof(addr));
	if(rc < 0) {
		pr_err("connect failed: %s\n", strerror(errno));
		close(fd);
		return rc;
	}
	pr_debug("client connected.\n");

	return fd;
}

