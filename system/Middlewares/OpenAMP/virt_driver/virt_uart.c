#ifdef VIRTIOCON

#include "platform_info.h"
#include "virt_uart.h"

#define RPMSG_SERVICE_NAME  "rpmsg-tty-raw"

static void *platform;
static struct rpmsg_device *rpdev;
static struct rpmsg_endpoint ept;
static virtio_buffer_t ring;

static int virt_uart_cb(struct rpmsg_endpoint *ept, void *data, size_t len,
			     uint32_t src, void *priv)
{
  virtio_buffer_write(&ring, data, len);
	return RPMSG_SUCCESS;
}

int virt_uart_open(void)
{
	int ret;

  // initial remoteproc platform
	ret = platform_init(0, NULL, &platform);
	if (!ret) {
    // create remoteproc rpmsg dev
		rpdev = platform_create_rpmsg_vdev(platform, 0,
              VIRTIO_DEV_SLAVE, NULL, NULL);
		if (rpdev) {
      // create rpmsg endpoint
			ret = rpmsg_create_ept(&ept, rpdev, RPMSG_SERVICE_NAME,
              0, RPMSG_ADDR_ANY, virt_uart_cb, NULL);
	    if (!ret) {
        // initial virtio ring buffer
			  virtio_buffer_init(&ring);
    	  return 0;
      }
			platform_release_rpmsg_vdev(rpdev);
		}
		platform_cleanup(platform);
	}

	return -1;
}

void virt_uart_close(void)
{
	rpmsg_destroy_ept(&ept);
	platform_release_rpmsg_vdev(rpdev);
	platform_cleanup(platform);
}

void virt_uart_poll(void)
{
	platform_poll(platform);
}

int virt_uart_read(void *buf, int len)
{
	virt_uart_poll();
	return virtio_buffer_read(&ring, buf, len);
}

int virt_uart_write(void *buf, int len)
{
  int wrote, left = len;

  while (left) {
    wrote = (left <= RPMSG_VRING_PAYLOAD_SIZE) ? left : RPMSG_VRING_PAYLOAD_SIZE;
    wrote = rpmsg_send(&ept, buf, wrote);
    if (wrote < 0) // error
      return wrote;
    buf += wrote;
    left -= wrote;
  }

  return len;
}

int virt_uart_available(void)
{
	virt_uart_poll();
  return virtio_buffer_read_available(&ring);
}

int virt_uart_peek(void)
{
  if (virt_uart_available() > 0) {
    uint8_t tmp;
    virtio_buffer_peek(&ring, &tmp, 1);
    return tmp;
  } else {
    return -1;
  }
}

#endif
