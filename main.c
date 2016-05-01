#include <libuvc/libuvc.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include <opencv2/highgui/highgui_c.h>

typedef struct _leap_t
{
    IplImage *left;
    IplImage *right;

    IplImage *i;

    pthread_mutex_t lock;
    
    uint64_t count;
} leap_t;

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

    pthread_mutex_unlock(&leap->lock);

    leap->count++;
}

int main(int argc, char **argv) {
    uvc_context_t *ctx;
    uvc_device_t *dev;
    uvc_device_handle_t *devh;
    uvc_stream_ctrl_t ctrl;
    uvc_error_t res;

    leap_t leap;

    cvNamedWindow("Left", CV_WINDOW_AUTOSIZE|CV_GUI_EXPANDED);
    cvNamedWindow("Right", CV_WINDOW_AUTOSIZE|CV_GUI_EXPANDED);

    memset(&leap, 0, sizeof(leap_t));

    res = uvc_init(&ctx, NULL);
    if (res < 0) {
        uvc_perror(res, "uvc_init");
        return res;
    }
    puts("UVC initialized");
    /* Locates the first attached UVC device, stores in dev */
    res = uvc_find_device(
            ctx, &dev,
            0, 0, NULL); /* filter devices: vendor_id, product_id, "serial_num" */
    if (res < 0) {
        uvc_perror(res, "uvc_find_device"); /* no devices found */
    } else {
        puts("Device found");
        /* Try to open the device: requires exclusive access */
        res = uvc_open(dev, &devh);
        if (res < 0) {
            uvc_perror(res, "uvc_open"); /* unable to open device */
        } else {
            uint16_t saturation;
            uint16_t x;

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

            uvc_get_saturation(devh, &saturation, UVC_GET_CUR);
            printf("Sat2: 0x%02X\n", saturation);

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

            uvc_print_diag(devh, stderr);

            res = uvc_get_stream_ctrl_format_size(devh, &ctrl, UVC_FRAME_FORMAT_ANY, 640, 480, 57);
            if(res < 0)
            {
                printf("stream_ctrl_format_size failed\n");
            }


            /* Print out the result */
            //printf("Stream control:\n");
            uvc_print_stream_ctrl(&ctrl, stderr);
            if (res < 0) {
                uvc_perror(res, "get_mode"); /* device doesn't provide a matching stream */
            } else {
                res = uvc_start_streaming(devh, &ctrl, cb, (void *)&leap, 0);
                if (res < 0) {
                    uvc_perror(res, "start_streaming"); /* unable to start stream */
                } else {
                    puts("Streaming...");

                    while(1)
                    {
                        if(leap.left != NULL) {
                          pthread_mutex_lock(&leap.lock);
                          cvShowImage("Left", leap.left);
                          cvShowImage("Right", leap.right);
                          pthread_mutex_unlock(&leap.lock);
                        }
                        cvWaitKey(33);
                    }

                    /* End the stream. Blocks until last callback is serviced */
                    uvc_stop_streaming(devh);
                    puts("Done streaming.");
                }
            }
            /* Release our handle on the device */
            uvc_close(devh);
            puts("Device closed");
        }
        /* Release the device descriptor */
        uvc_unref_device(dev);
    }
    /* Close the UVC context. This closes and cleans up any existing device handles,
     * and it closes the libusb context if one was not provided. */
    uvc_exit(ctx);
    puts("UVC exited");
    return 0;
}
