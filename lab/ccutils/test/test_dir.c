#include "../header.h"

int do_each(struct file_node *f)
{
	pr_info("%s\n", f->filename);

	return 0;
}

int main(void)
{
	dir_scan(".", do_each);

	return 0;
}
