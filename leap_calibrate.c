#include <stdio.h>
#include <stdlib.h>

#include "leap.h"

#include <opencv2/calib3d/calib3d.hpp>

int leap_calibrate_frame(leap_t *leap)
{
  int corner_count;
  CvPoint2D32f corners;

  cvFindChessboardCorners(leap->left, cvSize(8,6), &corners, &corner_count, CV_CALIB_CB_ADAPTIVE_THRESH|CV_CALIB_CB_NORMALIZE_IMAGE|CV_CALIB_CB_FAST_CHECK);

  printf("corners: %d\n", corner_count);

  return 0;
}
