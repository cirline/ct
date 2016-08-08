#define pr_fmt(fmt)	"sq_data " fmt

#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/seq_file.h>
#include <linux/proc_fs.h>

struct sq_data {
	struct list_head list;
	int value;
};

static struct mutex mlock;
static struct list_head sq_head;

static void data_add(int value)
{
	struct sq_data *data;

	mutex_lock(&mlock);
	data = kmalloc(sizeof(*data), GFP_KERNEL);
	if(data) {
		data->value = value;
		list_add(&data->list, &sq_head);
	}
	mutex_unlock(&mlock);

}

static ssize_t sq_write(struct file *file, const char __user *buffer,
		size_t count, loff_t *ppos)
{
	pr_info("%s\n", __func__);
	data_add(count);
	return count;
}

static int sq_show(struct seq_file *m, void *p)
{
	struct sq_data *data = list_entry(p, struct sq_data, list);

	pr_info("%s\n", __func__);
	seq_printf(m, "value: %d\n", data->value);
	return 0;
}

static void *sq_start(struct seq_file *m, loff_t *ppos)
{
	mutex_lock(&mlock);

	pr_info("%s\n", __func__);
	return seq_list_start(&sq_head, *ppos);
}

static void *sq_next(struct seq_file *m, void *p, loff_t *ppos)
{
	pr_info("%s\n", __func__);
	return seq_list_next(p, &sq_head, ppos);
}

static void sq_stop(struct seq_file *m, void *p)
{
	pr_info("%s\n", __func__);
	mutex_unlock(&mlock);
}

static struct seq_operations sq_sops = {
	.start = sq_start,
	.next = sq_next,
	.stop = sq_stop,
	.show = sq_show,
};

static int sq_open(struct inode *inode, struct file *file)
{
	pr_info("%s\n", __func__);
	return seq_open(file, &sq_sops);
}

static struct file_operations sq_fops = {
	.open = sq_open,
	.read = seq_read,
	.write = sq_write,
	.llseek = seq_lseek,
	.release = seq_release,
};

static void data_clean(struct list_head *head)
{
	struct sq_data *data;

	pr_info("%s\n", __func__);
	while(!list_empty(head)) {
		data = list_entry(head->next, struct sq_data, list);
		list_del(&data->list);
		kfree(data);
	}
}

static int __init sq_init(void)
{
	struct proc_dir_entry *proc;

	pr_info("%s\n", __func__);
	mutex_init(&mlock);
	INIT_LIST_HEAD(&sq_head);

	data_add(1);
	data_add(2);

	proc = proc_create("sq_data", S_IWUSR | S_IRUGO, NULL, &sq_fops);
	if(!proc) {
		data_clean(&sq_head);
		return - ENOMEM;
	}
	return 0;
}

static void __exit sq_exit(void)
{
	pr_info("%s\n", __func__);
	remove_proc_entry("sq_data", NULL);
	data_clean(&sq_head);
}

module_init(sq_init);
module_exit(sq_exit);

MODULE_LICENSE("GPL");

