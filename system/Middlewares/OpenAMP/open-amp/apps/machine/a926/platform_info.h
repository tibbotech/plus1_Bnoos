#ifndef PLATFORM_INFO_H_
#define PLATFORM_INFO_H_

#include <openamp/remoteproc.h>
#include <openamp/virtio.h>
#include <openamp/rpmsg.h>
#include "sp7021_arm926.h"
#include "xil_printf.h"

#if defined __cplusplus
extern "C" {
#endif

/* MBOX */
#define MBOX_TO_NOTIFY		(*(volatile unsigned int *)RF_GRP(259, 31)) /* MBOX_CA7_DIRECT[7] */
#define MBOX_NOTIFICATION	(*(volatile unsigned int *)RF_GRP(258, 31)) /* MBOX_A926_DIRECT[7] */
#define IRQ_NOTIFICATION	197 /* A926_DIRECT_INT7 */

/* Memory attributes */
#define NORM_NONCACHE 0x11DE2	/* Normal Non-cacheable */
#define STRONG_ORDERED 0xC02	/* Strongly ordered */
#define DEVICE_MEMORY 0xC06	/* Device memory */
#define RESERVED 0x0		/* reserved memory */


extern int __OPENAMP_region_start__[];  /* defined by linker script */
extern int __OPENAMP_region_end__[];    /* defined by linker script */


/* Shared memory */
#define SHARED_MEM_PA  ((metal_phys_addr_t)__OPENAMP_region_start__)
#define SHARED_MEM_SIZE (size_t)((void *)__OPENAMP_region_end__ - (void *) __OPENAMP_region_start__)
#define SHARED_BUF_OFFSET 0x80000UL

/* Remoteproc private data struct */
struct remoteproc_priv {
	atomic_int nokick; /* 0 for kick from other side */
};

/**
 * platform_init - initialize the platform
 *
 * It will initialize the platform.
 *
 * @argc: number of arguments
 * @argv: array of the input arguements
 * @platform: pointer to store the platform data pointer
 *
 * return 0 for success or negative value for failure
 */
int platform_init(int argc, char *argv[], void **platform);

/**
 * platform_create_rpmsg_vdev - create rpmsg vdev
 *
 * It will create rpmsg virtio device, and returns the rpmsg virtio
 * device pointer.
 *
 * @platform: pointer to the private data
 * @vdev_index: index of the virtio device, there can more than one vdev
 *              on the platform.
 * @role: virtio master or virtio slave of the vdev
 * @rst_cb: virtio device reset callback
 * @ns_bind_cb: rpmsg name service bind callback
 *
 * return pointer to the rpmsg virtio device
 */
struct rpmsg_device *
platform_create_rpmsg_vdev(void *platform, unsigned int vdev_index,
			   unsigned int role,
			   void (*rst_cb)(struct virtio_device *vdev),
			   rpmsg_ns_bind_cb ns_bind_cb);

/**
 * platform_poll - platform poll function
 *
 * @platform: pointer to the platform
 *
 * return negative value for errors, otherwise 0.
 */
int platform_poll(void *platform);

/**
 * platform_release_rpmsg_vdev - release rpmsg virtio device
 *
 * @rpdev: pointer to the rpmsg device
 */
void platform_release_rpmsg_vdev(struct rpmsg_device *rpdev);

/**
 * platform_cleanup - clean up the platform resource
 *
 * @platform: pointer to the platform
 */
void platform_cleanup(void *platform);

#if defined __cplusplus
}
#endif

#endif /* PLATFORM_INFO_H_ */
