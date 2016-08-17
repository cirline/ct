#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#include "taxicommon.h"
#include "taxihal.h"

#define LOG_TAG	"taxihal"
#include "cutils/log.h"

#define MODULE_NAME	"taxihal"
#define MODULE_AUTHOR	"chenqiwei@szhklt.com"

static int taxi_device_close(struct hw_device_t *devp)
{
	struct taxi_device_t *dev = (struct taxi_device_t *)devp;

	ALOGI("%s", __func__);
	if(dev) {
		close(dev->fd);
		free(dev);
	}

	return 0;
}

static int taxi_set_carstate(struct taxi_device_t *devp, int state)
{
	ALOGI("set state = %d", state);

	return 0;
}

static int taxi_get_carstate(struct taxi_device_t *devp, int * state_p)
{
	if(state_p)
		*state_p = 1;

	ALOGI("get state = %d", *state_p);

	return 0;
}

static int taxi_logsave(struct taxi_device_t *devp)
{
	ALOGI("%s", __func__);

	return 0;
}

static int taxi_device_open(const struct hw_module_t *module_p, const char *name, struct hw_device_t **devpp)
{
	struct taxi_device_t *dev;

	dev = malloc(sizeof(struct taxi_device_t));
	if(!dev) {
		ALOGE("malloc failed: %s", strerror(errno));
		return -ENOMEM;
	}

	memset(dev, 0, sizeof(*dev));
	dev->common.tag = HARDWARE_MODULE_TAG;
	dev->common.version = 0;
	dev->common.module = (struct hw_module_t *)module_p;
	dev->common.close = taxi_device_close;
	dev->set_carstate = taxi_set_carstate;
	dev->get_carstate = taxi_get_carstate;
	dev->logsave = taxi_logsave;

	dev->fd = open(DEV_IOCNTL_PATH, O_RDWR);
	if(dev->fd < 0) {
		int rc = dev->fd;
		ALOGE("open %s failed, %s", DEV_IOCNTL_PATH, strerror(errno));
		free(dev);
		return rc;
	}
	*devpp = &dev->common;
	ALOGI("open %s ok!", DEV_IOCNTL_PATH);

	return 0;
}

static struct hw_module_methods_t taxi_module_methods = {
	.open = taxi_device_open,
};

struct taxi_module_t HAL_MODULE_INFO_SYM = {
	.common = {
		.tag = HARDWARE_MODULE_TAG,
		.version_major = 1,
		.version_minor = 0,
		.id = TAXI_HARDWARE_MODULE_ID,
		.name = MODULE_NAME,
		.author = MODULE_AUTHOR,
		.methods = &taxi_module_methods,
	},
};

