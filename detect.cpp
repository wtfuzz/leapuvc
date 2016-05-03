#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "leap.h"

#include <opencv2/opencv.hpp>

#include <opencv2/nonfree/features2d.hpp>

using namespace cv;

Mat display_left(480,640, CV_8UC3, Scalar(0,0,255));
Mat display_right(480,640, CV_8UC3, Scalar(0,0,255));
Mat display_match(480,640, CV_8UC3, Scalar(0,0,255));

Mat map(1000,1000, CV_8UC3, Scalar(0,0,0));

void render_callback(leap_t *leap, int key)
{
  //imshow("left", display_left);
  //imshow("right", display_right);
  imshow("match", display_match);

  //imshow("map", map);
}

void callback(leap_t *leap)
{
  //FeatureDetector *feat = FeatureDetector::create("SURF");
  //FeatureDetector *detector = FeatureDetector::create("FAST");
  FastFeatureDetector detector(6);
  //OrbFeatureDetector detector(5);
  //SurfFeatureDetector detector(20);
  SurfDescriptorExtractor ext;
  FlannBasedMatcher matcher;
  //BFMatcher matcher;

  Mat left(leap->left);
  Mat right(leap->right);
  
  vector <DMatch> matches;
  vector <DMatch> good_matches;

  vector <KeyPoint> keypoints_left; 
  vector <KeyPoint> keypoints_right; 

  Mat descriptors_left, descriptors_right;

  map -= Scalar(20,20,20);

  //FAST(left, keypoints_left, 8, true);
  //FAST(right, keypoints_right, 8, true);

  detector.detect(left, keypoints_left);
  detector.detect(right, keypoints_right);

  if(keypoints_left.size() == 0)
    return;
  if(keypoints_right.size() == 0)
    return;

  ext.compute(left, keypoints_left, descriptors_left);
  ext.compute(right, keypoints_right, descriptors_right);

  matcher.match(descriptors_left, descriptors_right, matches);

  if(matches.size() == 0)
    return;

  //drawKeypoints(left, keypoints_left, display_left);
  //drawKeypoints(right, keypoints_right, display_right);

  double max_dist = 0; double min_dist = 1000;

  //-- Quick calculation of max and min distances between keypoints
  for( int i = 0; i < descriptors_left.rows; i++ )
  { double dist = matches[i].distance;
    if( dist < min_dist ) min_dist = dist;
    if( dist > max_dist ) max_dist = dist;
  }
 
  for( int i = 0; i < (int)matches.size(); i++ )
  {
    int kp1 = matches[i].queryIdx;
    int kp2 = matches[i].trainIdx;
    float dx,dy;

    KeyPoint p1,p2;

    //printf( "Good Match [%d] Keypoint 1: %d  -- Keypoint 2: %d  \n", i, good_matches[i].queryIdx, good_matches[i].trainIdx );

    p1 = keypoints_left[kp1];
    p2 = keypoints_right[kp2];
    //printf("%f,%f -> %f,%f\n", p1.pt.x, p1.pt.y, p2.pt.x, p2.pt.y);

    dx = p1.pt.x - p2.pt.x;
    dy = p1.pt.y - p2.pt.y;

    if(fabs(dy) <= 10 && fabs(dx) > 20)
    {
      float distance = (fabs(dx)/200.0)*100.0;
      //printf("%f\n", fabs(dx));
      good_matches.push_back(matches[i]);


      //printf("%f\n", distance);

      // Draw on the map
      circle(map, Point(500-p1.pt.x,1000-distance*10), 3, Scalar(0,255,0));

    }
   
    //if( matches[i].distance <= max(2*min_dist, 0.001) )
      //good_matches.push_back( matches[i]);
  }

  if(good_matches.size() > 0)
    drawMatches(left, keypoints_left, right, keypoints_right, good_matches, display_match);
 

}

int main()
{
  leap_t *leap;
  leap_open(&leap, &callback);
  leap_diag(leap);

  leap->render_callback = render_callback;

  leap_run(leap); 
}
