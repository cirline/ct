#ifndef __UTILS_UFILE_H__
#define __UTILS_UFILE_H__

#include <dirent.h>

struct dir_node {
	struct dir_node *prev;

	DIR *d;
	char *path;
};

struct file_node {
	char * filename;
	char * rel_dir;
};

typedef int (*fn_do_each_t)(struct file_node *f);

extern int file_type(const char *path);
extern int dir_scan(const char *, fn_do_each_t do_each);
extern struct file_node * new_file_node(struct file_node *node);
extern void free_file_node(struct file_node *node);

#endif
