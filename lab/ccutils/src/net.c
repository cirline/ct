//#define DEBUG
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <curl/curl.h>

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

static size_t cc_http_get_buffer(void *ptr, size_t size, size_t nmemb, void *stream)
{
	size_t length;

	length = size * nmemb;
	memcpy(stream, ptr, length);
	return length;
}

int http_get(const char *url, char *buffer)
{
	CURL *curl_handle;
	double dsize = 0;
	CURLcode res;

	if(!url) {
		pr_err("%s, url = null\n");
		return -1;
	}

	curl_handle = curl_easy_init();
	if(curl_handle) {
		curl_easy_setopt(curl_handle, CURLOPT_URL, url);
		curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, cc_http_get_buffer);
		curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, buffer);
		res = curl_easy_perform(curl_handle);

		res = curl_easy_getinfo(curl_handle, CURLINFO_SIZE_DOWNLOAD, &dsize);
		if(res != CURLE_OK) {
			pr_err("get dsize failed: %s\n", curl_easy_strerror(res));
		}

		curl_easy_cleanup(curl_handle);
	}

	return dsize;
}

#if 0
int http_get(const char *url, char *buffer, int buffer_len)
{
	char local_url[1024];
	char get_url[1024];
	struct hostent *phost;
	char *p;
	struct sockaddr_in saddr;
	int fd;
	int rc;

	strcpy(local_url, url);
	for(p = local_url; *p != '/' && *p != 0; p++)
		;

	strcpy(get_url, p);
	pr_debug("get_url = %s\n", get_url);
	*p = 0;

	phost = gethostbyname(local_url);
	if(!phost) {
		pr_err("gethostbyname = null\n");
		return -1;
	}
	memset(&saddr, 0, sizeof(struct sockaddr_in));
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = *((unsigned long *)phost->h_addr_list[0]);
	saddr.sin_port = htons(80);

	sprintf(buffer, "GET %s HTTP/1.1\r\nHOST: %s\r\nCache-Control: no-cache\r\n\r\n",
			get_url, local_url);
	pr_debug("get string:\n%s", buffer);

	fd = socket(AF_INET, SOCK_STREAM, 0);
	if(fd < 0) {
		pr_err("socket failed: %s\n", strerror(errno));
		return fd;
	}

	rc = connect(fd, (struct sockaddr *)&saddr, (socklen_t)sizeof(struct sockaddr_in));
	if(rc < 0) {
		pr_err("connect failed: %s\n", strerror(errno));
		close(fd);
		return rc;
	}

	rc = send(fd, buffer, strlen(buffer), 0);
	if(rc < 0) {
		pr_err("send failed: %s\n", strerror(errno));
		close(fd);
		return rc;
	}
	rc = recv(fd, buffer, buffer_len, 0);
	if(rc < 0) {
		pr_err("recv failed: %s\n", strerror(errno));
		close(fd);
		return rc;
	}
	pr_debug("recv:\n%s", buffer);

	return rc;
}
#endif

char *split_http_response_header(char *buffer)
{
	char *p;

	for(p = buffer; *p != 0; p++) {
		if(*p != '\r')
			continue;
		p++;
		if(*p != '\n')
			continue;
		p++;
		if(*p != '\r')
			continue;
		p++;
		if(*p != '\n')
			continue;

		return p + 1;
	}

	return NULL;
}

