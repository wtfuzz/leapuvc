#include <libuvc/libuvc.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "leap.h"

#include <opencv2/highgui/highgui_c.h>

static void process(leap_t *leap)
{
  cvAdd(leap->left, leap->right, leap->i, NULL);
}

int main(int argc, char **argv) {
  leap_t *leap;

  leap_open(&leap);
  leap_diag(leap);

  while(1)
  {
    int key;
    if(leap->left != NULL) {
      pthread_mutex_lock(&leap->lock);
      cvShowImage("Left", leap->left);
      cvShowImage("Right", leap->right);
      cvShowImage("i", leap->i);
      pthread_mutex_unlock(&leap->lock);
    }

    key = cvWaitKey(33);

    if(key > 0)
      break;
  }

  leap_close(leap);

  return EXIT_SUCCESS;

}
