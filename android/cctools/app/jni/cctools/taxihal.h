#ifndef __TAXI_HAL_H__
#define __TAXI_HAL_H__

#include <hardware/hardware.h>

#define TAXI_HARDWARE_MODULE_ID	"taxihal"

struct taxi_module_t {
	struct hw_module_t common;
};

struct taxi_device_t {
	struct hw_device_t common;
	int fd;
	int (*set_carstate)(struct taxi_device_t *devp, int state);
	int (*get_carstate)(struct taxi_device_t *devp, int* state_p);

	int (*logsave)(struct taxi_device_t *devp);
};

#endif
