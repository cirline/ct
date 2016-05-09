#define DEBUG
#define pr_fmt(fmt)	"ToolsFS"

#include "../utils/header.h"
#include "header.h"

#define FILESYNC_PATH	"/mnt/sdcard/documents"

#define FILE_TABLE_SIZE		1024

static struct table_node * tn_table[FILE_TABLE_SIZE];
static struct hash_table_desc htdesc;

void ht_print(int i, void *p)
{
	struct file_node *node = p;
	pr_info("%4d, %s\n", i, node->filename);
}

int build_node(struct table_node *tnode, struct file_node *fnode, char *name)
{
	fnode->filename = name;

	memset(tnode, 0, sizeof(*tnode));
	tnode->p = fnode;
	tnode->cmp_str = name;
	tnode->hash = strhash(name, FILE_TABLE_SIZE);

	return 0;
}

int do_each_file(struct file_node *node)
{
	struct table_node tnode;
	struct file_node fnode;

	build_node(&tnode, &fnode, node->filename);
	hash_table_insert(&htdesc, &tnode);

	return 0;
}

int socket_test(void)
{
	int fd;
	char buffer[1024];
	int rc;

	fd = new_client_socket(SERVER_IP, 9877);
	if(fd < 0) {
		pr_err("new client socket failed.");
		return -1;
	}

	while(1) {
		read(fd, buffer, 1024);
		pr_info("buffer: %s\n", buffer);
		if(strcmp(buffer, "command:end") == 0)
			break;
		rc = 1;
		write(fd, &rc, sizeof(rc));
	}

	pr_info("%s\n", buffer);

	return 0;
}


int fsscan(void)
{
	htdesc.tbl = tn_table;
	htdesc.size = FILE_TABLE_SIZE;
	htdesc.insert = (void *(*)(void *))new_file_node;
	htdesc.remove = (void (*)(void *))free_file_node;
	htdesc.print = ht_print;

	dir_scan(FILESYNC_PATH, do_each_file);

	hash_table_print(&htdesc);

	socket_test();

	return 0;

}


