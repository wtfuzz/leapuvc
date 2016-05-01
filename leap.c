#include <pthread.h>
#include "leap.h"

void cb(uvc_frame_t *frame, void *ptr) {
    leap_t *leap = (leap_t *)ptr;
    uvc_error_t ret;
    uint8_t *data = (uint8_t *)frame->data;

    if(leap->left == NULL)
    {
        leap->left = cvCreateImage(
                cvSize(frame->width, frame->height),
                IPL_DEPTH_8U,
                1);

        leap->right = cvCreateImage(
                cvSize(frame->width, frame->height),
                IPL_DEPTH_8U,
                1);

        leap->i = cvCreateImage(
          cvSize(frame->width, frame->height),
          IPL_DEPTH_8U,
          1);
    }

    pthread_mutex_lock(&leap->lock);

    {
        int i,j;
        for(i=0;i<frame->height;i++)
        {
            for(j=0;j<frame->width;j++)
            {
                leap->left->imageData[j+i*frame->width] = data[j*2+i*frame->width*2];
                leap->right->imageData[j+i*frame->width] = data[j*2+1+i*frame->width*2];
            }
        }
    }

    //process(leap);

    pthread_mutex_unlock(&leap->lock);

    leap->count++;
}

static int _leap_init(leap_t *leap)
{
  uint16_t saturation;
  uint16_t x;

  uvc_device_handle_t *devh = leap->devh;  

  uvc_set_white_balance_temperature(devh, 0x7f);

  for(x=0x00;x<=0x0d;x++)
    uvc_set_sharpness(devh, x);
  for(x=0x64;x<=0xff;x++)
    uvc_set_sharpness(devh, x);

  uvc_set_sharpness(devh, 0x62);
  uvc_set_sharpness(devh, 0x63);

  uvc_set_sharpness(devh, 0x10);
  uvc_set_saturation(devh, 0x6e);
  uvc_set_sharpness(devh, 0x11);
  uvc_set_saturation(devh, 0x31);
  uvc_set_sharpness(devh, 0x12);
  uvc_set_saturation(devh, 0x0c);
  uvc_set_sharpness(devh, 0x13);
  uvc_set_saturation(devh, 0xfa);
  uvc_set_sharpness(devh, 0x2c);
  uvc_set_saturation(devh, 0x2a);
  uvc_set_sharpness(devh, 0x2d);
  uvc_set_saturation(devh, 0x79);
  uvc_set_sharpness(devh, 0x2e);
  uvc_set_saturation(devh, 0x17);
  uvc_set_sharpness(devh, 0x2f);
  uvc_set_saturation(devh, 0x8b);
  uvc_set_sharpness(devh, 0x14);

  for(x=0x15;x<=0x20;x++)
    uvc_set_sharpness(devh, x);

  uvc_set_saturation(devh, 0x1f);
  uvc_set_sharpness(devh, 0x21);
  uvc_set_saturation(devh, 0x12);
  uvc_set_sharpness(devh, 0x22);
  uvc_set_saturation(devh, 0x8c);
  uvc_set_sharpness(devh, 0x23);
  uvc_set_saturation(devh, 0x4d);
  uvc_set_sharpness(devh, 0x24);
  uvc_set_saturation(devh, 0xcc);
  uvc_set_sharpness(devh, 0x25);
  uvc_set_saturation(devh, 0xe1);
  uvc_set_sharpness(devh, 0x26);
  uvc_set_saturation(devh, 0x8d);
  uvc_set_sharpness(devh, 0x27);

  uvc_set_saturation(devh, 0x92);
  uvc_set_sharpness(devh, 0x62);

  uvc_set_sharpness(devh, 0x63);

  uvc_set_focus_abs(devh, 0x03e8);
  uvc_set_contrast(devh, 0x01);
  uvc_set_brightness(devh, 0x04);
  uvc_set_focus_abs(devh, 0x03e8);
  uvc_set_zoom_abs(devh, 0xc8);
  uvc_set_gain(devh, 0x10);
  uvc_set_gamma(devh, 0x01);
  uvc_set_contrast(devh, 0x00);
  uvc_set_contrast(devh, 0x42);
  uvc_set_contrast(devh, 0x43);
  uvc_set_contrast(devh, 0x44);
  uvc_set_contrast(devh, 0x06);
  uvc_set_contrast(devh, 0x3c05);
  uvc_set_white_balance_temperature(devh, 0x7f);

  for(x=0x00;x<=0x0d;x++)
    uvc_set_sharpness(devh, x);

  uvc_set_sharpness(devh, 0x62);
  uvc_set_sharpness(devh, 0x63);

  return 0;
}

void leap_diag(leap_t *leap)
{
  uvc_print_diag(leap->devh, stderr);
}

int leap_open(leap_t **leap)
{
  uvc_stream_ctrl_t ctrl;
  uvc_error_t res;

  *leap = (leap_t *)calloc(1, sizeof(leap_t));

  res = uvc_init(&(*leap)->ctx, NULL);
  if (res < 0) {
      uvc_perror(res, "uvc_init");
      return -1;
  }
  res = uvc_find_device(
    (*leap)->ctx, &(*leap)->dev,
    0, 0, NULL); /* filter devices: vendor_id, product_id, "serial_num" */
  if(res < 0)
    return -1;

  res = uvc_open((*leap)->dev, &(*leap)->devh);

  _leap_init(*leap);

  res = uvc_get_stream_ctrl_format_size((*leap)->devh, &ctrl, UVC_FRAME_FORMAT_ANY, 640, 480, 57);

  res = uvc_start_streaming((*leap)->devh, &ctrl, cb, (void *)*leap, 0);
  
  return 0;
}

int leap_close(leap_t *leap)
{
  uvc_close(leap->devh);
  uvc_unref_device(leap->dev);
  uvc_exit(leap->ctx);
  return 0;
}
