#include <linux/module.h>
#include <linux/proc_fs.h>


static int proc_file_pread(char *buf, char **start, off_t offset, int count, int *eof, void *data)
{
    char *string = "hello proc read";
    int i;

    for(i = 0; *(string + i); i++) {
        buf[i] = string[i];
    }
    return i;
}

static int __init proc_file_init(void)
{
	printk(KERN_INFO"hello ---------add 0934------------------pc module\n");

    create_proc_read_entry("proc_file_read", 0, NULL, proc_file_pread, NULL);
	return 0;
}

static void __exit proc_file_exit(void)
{
    remove_proc_entry("proc_file_read", NULL);
	printk(KERN_INFO"bye kernel.\n");
}

module_init(proc_file_init);
module_exit(proc_file_exit);
MODULE_LICENSE("GPL");


