#include <stdio.h>
#include <stdlib.h>

#include "leap.h"

#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/calib3d/calib3d.hpp>

int leap_calibrate_frame(leap_t *leap)
{
  int found = 0;
  int corner_count;
  CvSize pattern_size = cvSize(9,6);
  CvPoint2D32f corners[128];

  found = cvFindChessboardCorners(leap->left, pattern_size, corners, &corner_count, CV_CALIB_CB_ADAPTIVE_THRESH|CV_CALIB_CB_NORMALIZE_IMAGE|CV_CALIB_CB_FAST_CHECK);

  if(found)
  {

    cvCvtColor(leap->left, leap->i, CV_GRAY2RGB);

    cvDrawChessboardCorners(
      leap->i,
      pattern_size,
      corners,
      corner_count,
      found);
  }

  printf("found: %d corners: %d\n", found, corner_count);

  return 0;
}
