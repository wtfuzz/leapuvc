#include <libuvc/libuvc.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "leap.h"

#include <opencv2/highgui/highgui_c.h>
#include <opencv2/imgproc/imgproc_c.h>

int translate_x = 50;
int tmin = 180;
int tmax = 255;

static void process(leap_t *leap)
{
  // Translation matrix
  static CvMat *M = NULL;
  
  if(M == NULL)
  {
    M = cvCreateMat(2,3,CV_32F);
    cvSetIdentity(M, cvRealScalar(1));
  }

  // x translation
  cvSet2D(M, 0,2, cvRealScalar(translate_x));
  // y translation
  cvSet2D(M, 1,2, cvRealScalar(0));

  cvWarpAffine(leap->right, leap->right, M, CV_INTER_LINEAR|CV_WARP_FILL_OUTLIERS, cvScalarAll(0));

  cvCanny(leap->left, leap->left, 10, 50, 3);
  cvCanny(leap->right, leap->right, 10, 50, 3);

  cvAdd(leap->left, leap->right, leap->work, NULL);
  
  //cvThreshold(leap->work, leap->i, tmin, tmax, CV_THRESH_BINARY);


}

void callback(leap_t *leap)
{
  process(leap);
  //leap_calibrate_frame(leap);
}

int main(int argc, char **argv) {
  leap_t *leap;

  cvNamedWindow("i", 1);
  cvCreateTrackbar("X", "i", &translate_x, 1000, NULL);
  cvCreateTrackbar("Tmin", "i", &tmin, 255, NULL);

  leap_open(&leap, &callback);
  leap_diag(leap);

  while(1)
  {
    int key;
    pthread_mutex_lock(&leap->lock);

    if(leap->left != NULL) {
      cvShowImage("Left", leap->left);
      cvShowImage("Right", leap->right);
      cvShowImage("work", leap->work);
      cvShowImage("i", leap->i);
    }

    key = cvWaitKey(33);
    pthread_mutex_unlock(&leap->lock);

    if(key > 0)
      break;
  }

  leap_close(leap);

  return EXIT_SUCCESS;

}
