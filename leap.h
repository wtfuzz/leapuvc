#ifndef LEAP_H_
#define LEAP_H_

#include <libuvc/libuvc.h>
#include <opencv2/core/core_c.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _leap_t leap_t;
typedef void (*leap_callback_t)(leap_t *);
typedef void (*leap_render_callback_t)(leap_t *, int key);

struct _leap_t
{
  uvc_context_t *ctx;
  uvc_device_t *dev;
  uvc_device_handle_t *devh;

  IplImage *left;
  IplImage *right;

  IplImage *work;

  IplImage *i;

  pthread_mutex_t lock;

  leap_callback_t callback;
  leap_render_callback_t render_callback;

  uint64_t count;
};


int leap_open(leap_t **leap, leap_callback_t callback);
int leap_close(leap_t *leap);
void leap_diag(leap_t *leap);
int leap_run(leap_t *leap);

int leap_calibrate_frame(leap_t *leap);

#ifdef __cplusplus
}
#endif

#endif
