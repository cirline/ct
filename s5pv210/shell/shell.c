/**
 * shell function type: int (*)(void *)
 */

#include <common.h>

int shell_dump_registers(void *p)
{
	pr_info("%s\n", __func__);

	dump_stack_frame();
	__asm__ __volatile__("push {r0-r4}" : :);
	dump_stack_frame();
	__asm__ __volatile__("pop {r0-r4}" : :);
	dump_stack_frame();

	pr_info("dump_registers\n");
	dump_registers();
	dump_stack_frame();

	return 0;
}
