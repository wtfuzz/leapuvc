#ifndef LEAP_H_
#define LEAP_H_

#include <libuvc/libuvc.h>
#include <opencv2/core/core_c.h>

typedef struct _leap_t
{
  uvc_context_t *ctx;
  uvc_device_t *dev;
  uvc_device_handle_t *devh;

  IplImage *left;
  IplImage *right;

  IplImage *i;

  pthread_mutex_t lock;

  uint64_t count;
} leap_t;

int leap_open(leap_t **leap);
int leap_close(leap_t *leap);
void leap_diag(leap_t *leap);

#endif
