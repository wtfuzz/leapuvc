#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "leap.h"

#include <opencv2/opencv.hpp>

#define NUM_FRAMES 30

using namespace cv;

bool found;
Size patternsize(4,11);
Size imagesize(480,640);

Mat distCoeffs = Mat::zeros(8, 1, CV_64F);
Mat cameraMatrix;

bool calibrated = false;

float square_size = 1.0;

Mat display(480,640, CV_8UC3, Scalar(0,0,255));
vector<Point2f> centers;
vector<vector<Point2f> > points;

static void calcCorners(Size boardSize, float squareSize, vector<Point3f>& corners)
{
    corners.resize(0);
    
    for( int i = 0; i < boardSize.height; i++ )
        for( int j = 0; j < boardSize.width; j++ )
            corners.push_back(Point3f(float((2*j + i % 2)*squareSize),
                                      float(i*squareSize), 0));
}

void run_calibration()
{
  vector<Point3f> corners;
  vector<vector<Point3f> > objectPoints(1);

  vector<Mat> rvecs;
  vector<Mat> tvecs;

  printf("Running calibration\n");

  calcCorners(patternsize, square_size, objectPoints[0]);

  objectPoints.resize(points.size(), objectPoints[0]);

 
  double rms = calibrateCamera(objectPoints, points, imagesize, cameraMatrix,
                                   distCoeffs, rvecs, tvecs, CV_CALIB_FIX_K4|CV_CALIB_FIX_K5);
                                                              ///*|CV_CALIB_FIX_K3*/|CV_CALIB_FIX_K4|CV_CALIB_FIX_K5);
  printf("RMS error reported by calibrateCamera: %g\n", rms); 

  calibrated = true;

}

void render_callback(leap_t *leap, int key)
{
  Mat raw_left(leap->left);
  Mat raw_right(leap->right);

  if(key >= 0)
  {
    if(key == 'c' && found == true)
    {
      points.push_back(centers);

      printf("Captured %lu/%d\n", points.size(), NUM_FRAMES);

      if(points.size() >= NUM_FRAMES)
      {
        run_calibration();
      }
    }
  }

  imshow("left", raw_left);
  imshow("right", raw_right);
  imshow("display", display);
}

void callback(leap_t *leap)
{
  Mat fixed(480,640, CV_8UC1, Scalar(0));
  Mat left(leap->left);

  if(calibrated == true)
  {
    undistort(left, fixed, cameraMatrix, distCoeffs);
    cvtColor(fixed, display, CV_GRAY2RGB);
  }
  else
  {
    found = findCirclesGrid(left, patternsize, centers, CALIB_CB_ASYMMETRIC_GRID);
    cvtColor(left, display, CV_GRAY2RGB);
    drawChessboardCorners(display, patternsize, Mat(centers), found);
  }

}

int main()
{
  leap_t *leap;
  leap_open(&leap, &callback);
  leap_diag(leap);

  leap->render_callback = render_callback;

  leap_run(leap); 
}
