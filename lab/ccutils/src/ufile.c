#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>

#include <sys/stat.h>

#include "header.h"

/**
 * test file type
 * <1: error
 * =0: dir
 * >0: file
 */
int file_type(const char *path)
{
	struct stat sbuf;

	if(stat(path, &sbuf)) {
		pr_err("stat %s failed: %s\n", strerror(errno));
		return -1;
	}

	if(S_ISDIR(sbuf.st_mode)) {
		return 0;
	} else {
		return 1;
	}
}

struct dir_node * new_dir_node(struct dir_node *parent, const char * path)
{
	struct dir_node *node;

	node = malloc(sizeof(struct dir_node));
	if(!node) {
		pr_err("malloc node failed: %s\n", strerror(errno));
		goto err;
	}
	node->path = strdup(path);
	if(!node->path) {
		pr_err("dup path failed: %s\n", strerror(errno));
		goto err_dup_path;
	}

	node->prev = parent;
	node->d = opendir(path);
	if(!node->d) {
		pr_err("open dir %s failed: %s\n", path, strerror(errno));
		free(node);
		return NULL;
	}

	return node;

err_dup_path:
	free(node);
err:
	return NULL;
}

struct dir_node * put_dir_node(struct dir_node *node)
{
	struct dir_node *p;

	if(!node)
		return NULL;

	closedir(node->d);
	p = node->prev;
	free(node);

	return p;
}

/**
 * dir_scan - scan a dir and execute the fn_do_each_t
 * @root: the scan root directory
 * @do_each: execute function
 */
int dir_scan(const char * root, fn_do_each_t do_each)
{
	struct dirent *dir;
	int count = 0;
	struct dir_node *dnode;
	char cur[1024];
	int type;
	struct file_node fnode;

	dnode = new_dir_node(NULL, root);

	while(dnode) {
		while((dir = readdir(dnode->d))) {
			if(*dir->d_name  == '.')
				continue;

			sprintf(cur, "%s/%s", dnode->path, dir->d_name);
			type = file_type(cur);

			if(type == 0) {
				pr_debug("dir: %s\n", cur);
				break;
			}
			fnode.filename = dir->d_name;
			do_each(&fnode);
		}

		if(!dir)
			dnode = put_dir_node(dnode);
		else
			dnode = new_dir_node(dnode, cur);
	}

	return count;
}

struct file_node * new_file_node(struct file_node *node)
{
	struct file_node *new;

	if(!node)
		return NULL;

	new = malloc(sizeof(*new));
	if(!new) {
		pr_err("malloc file node failed: %s\n", strerror(errno));
		return NULL;
	}
	new->filename = strdup(node->filename);

	return new;
}

void free_file_node(struct file_node *node)
{
	if(node) {
		free(node->filename);
		free(node);
	}
}

