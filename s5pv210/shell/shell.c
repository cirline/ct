/**
 * shell function type: int (*)(void *)
 */

#include <common.h>

int shell_dump_registers(void *p)
{
	pr_info("%s\n", __func__);

	dump_registers();

	return 0;
}
